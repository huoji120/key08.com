/******************************************************************
 ģ�����ƣ����ݿ�����ࣻ
 ʵ�ֹ��ܣ��ṩ�ӿڣ�ʵ�����ݵ��������Ͳ�����
 �о���Ա�������Σ�
 �����£�2010-05-12

 Ԥ�Ȳ�����
 ��1����stdafx.h�����
    #import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
 ��2���������̵�InitInstance()�����
         if(!AfxOleInit())
        {
         AfxMessageBox("OLE��ʼ������");
         return FALSE;
         }

 ��3����.cpp�ļ���Ҫ����"CPFile.h"
*********************************************************************/
/**************************************************************
CPFile.cpp
**************************************************************/
#include "stdafx.h"
//////////////////////////////////////////////////////////////////////
// ���캯��
//////////////////////////////////////////////////////////////////////
//Ĭ�ϵĹ��캯��
CPData::CPData()
{
    //��ʼ��
    m_nResultRow = 0;
    m_nResultCol=0;
    m_pConnection = NULL;
    //��������
    m_pRecordset.CreateInstance(_uuidof(Recordset));
    m_pCommand.CreateInstance(_uuidof(Command));
}

//��������Ĺ��캯��
CPData::CPData(_ConnectionPtr pConnection)
{
    m_pConnection = pConnection;
    m_nResultRow = 0;
    m_nResultCol=0;
    //��������
    m_pRecordset.CreateInstance(_uuidof(Recordset));
    m_pCommand.CreateInstance(_uuidof(Command));

}
//////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////
CPData::~CPData()
{
    if(m_pRecordset->State!=adStateClosed)
        m_pRecordset->Close();
    m_pRecordset = NULL;

    if(m_pCommand->State!=adStateClosed)
        m_pCommand->Release();
    m_pCommand = NULL;

    if(m_pConnection->State!=adStateClosed)
        m_pConnection->Close();
    m_pConnection = NULL;
}

/////////////////////////////////////////////////////////////////////
///�򵥲�������
////////////////////////////////////////////////////////////////////

//�õ��������������
long CPData::GetResultRow()
{
    return this->m_nResultRow;
}

//�õ��������������
long CPData::GetResultCol()
{
    return this->m_nResultCol;
}

//�õ��������
_RecordsetPtr CPData::GetResult()
{
    return this->m_pRecordset;
}

BOOL CPData::Create(CString strFileName)
{
	CoInitialize(NULL);
	HRESULT hr = S_OK;
	
	CString strcnn ( _T("Provider=Microsoft.JET.OLEDB.4.0;Data source ="+ strFileName));
	try
	{
		ADOX::_CatalogPtr m_pCatalog = NULL;
		hr = m_pCatalog.CreateInstance(__uuidof (ADOX::Catalog));
		if(FAILED(hr))
		{
			_com_issue_error(hr);
		}
		else
		{
			m_pCatalog->Create(_bstr_t(strcnn));
		}
	}
	catch(_com_error &e)
	{
		// Notify the user of errors if any.
		AfxMessageBox(e.ErrorMessage());
		return FALSE;
    }
	return TRUE;
}
///////////////////////////////////////////////////////////////////////
///���Ӳ���
///////////////////////////////////////////////////////////////////////

//���ӵ����ݿ�
//1.�����ַ��������Լ�����,Ҳ���Դ��ļ��ж���
BOOL CPData::Connect(CString strFileName)
{
    try{
        m_pConnection.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source="+ strFileName +";Persist Security Info=False";

        HRESULT hr;

		hr = m_pConnection->Open(strConnect,"","",adModeUnknown);

        //��������
        //����ʧ��
        if(FAILED(hr))
        {
            AfxMessageBox("Connect Error!");
            return FALSE;
        }
    }
    catch(_com_error&e)
    {
        AfxMessageBox(e.Description()+"B");
        return FALSE;
    }
    return TRUE;
}

//�Ͽ�����
void CPData::DisConnect()
{
    if(m_pConnection->State!=adStateClosed)
        m_pConnection->Close();
}

///////////////////////////////////////////////////////////////////////
///���²���
///////////////////////////////////////////////////////////////////////
BOOL CPData::Execute(CString strSql)
{
    try
    {
        _variant_t vNULL;
        vNULL.vt = VT_ERROR;

        ///����Ϊ�޲���
        vNULL.scode = DISP_E_PARAMNOTFOUND;

        ///�ǳ��ؼ���һ�䣬�����������Ӹ�ֵ����
        m_pCommand->ActiveConnection = m_pConnection;

        ///�����ִ�
        m_pCommand->CommandText = (_bstr_t)strSql;

        ///ִ�����ȡ�ü�¼��
        m_pRecordset = m_pCommand->Execute(&vNULL,&vNULL,adCmdText);

        //ȷʵ��vNULL�е�intVal��ִ�в�����Ӱ�������
        m_nResultRow = 0;
        m_nResultRow = vNULL.intVal;
    }
    catch(_com_error e)
    {
        m_nResultRow = 0;
        return FALSE;
    }
    return TRUE;
}


//�ж��ֶδ���
bool CPData::CheckFields(CString strSql)
{
	try
	{
		m_pRecordset->CursorLocation=adUseClient;    //�����α�λ��,����Ϊ�ͻ�����ʽ,����GetRecordCount()����ֵ����
		m_pRecordset->Open(_variant_t(strSql),_variant_t((IDispatch *)m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
		m_pRecordset->Close();
	}
	catch(_com_error)
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////
///��ѯ����
///////////////////////////////////////////////////////////////////////
int CPData::Select(CString strSql)
{
    try
    {
        m_nResultCol=0;
        m_nResultRow=0;
        m_pRecordset->CursorLocation=adUseClient;    //�����α�λ��,����Ϊ�ͻ�����ʽ,����GetRecordCount()����ֵ����
        m_pRecordset->Open(_variant_t(strSql),_variant_t((IDispatch *)m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
        m_nResultCol = m_pRecordset->Fields->GetCount();//�õ���ѯ���������
        m_nResultRow = m_pRecordset->GetRecordCount();  //�õ���ѯ���������
    }
    catch(_com_error&e)
    {
        AfxMessageBox(e.Description()+"D");
        return -1;
    }
    return 1;
}

//��ѯ��䣬����Խ�����ѯһ���ֶε�������д���
//��������CStringArray���͵ı���pResult��
int CPData::Select(CString strSql,CStringArray& Result)
{
    if(Select(strSql)!=0)
    {
        Result.RemoveAll();
        _variant_t value;
        for(int i=0;i<m_nResultRow;i++)
        {
            for(int j=0;j<m_nResultCol;j++)
            {
                value=m_pRecordset->Fields->Item[(long)(/*i*m_nResultCol+*/j)]->Value;
                if(value.vt==3||value.vt==14)
                {
                    CString strTrans;
                    strTrans.Format("%ld",value.intVal);
                    Result.Add(strTrans);
                }else if(value.vt==7)
                {
					COleDateTime time = value.date;
                    CString strTemp;
                    strTemp.Format("%d-%d-%d %s",time.GetYear(),time.GetMonth(),time.GetDay(),time.Format("%H:%M:%S"));
                    Result.Add(strTemp);
                }else{
					CString strTemp(value.bstrVal);
					Result.Add( strTemp );
				}
            }
            m_pRecordset->MoveNext();
        }
        m_pRecordset->Close();
        return m_nResultCol;
    }
    else
    {
        m_pRecordset->Close();
        return -1;
    }
}

BOOL CPData::SelectMulitCol(CString strSql,CStringArray& Result)
{
    if(Select(strSql)!=0)
    {
        Result.RemoveAll();
        _variant_t value;
        for(int i=0;i<m_nResultRow;i++)
        {
            for(int j=0;j<m_nResultCol;j++)
            {
                value=m_pRecordset->Fields->Item[(long)(/*i*m_nResultCol+*/j)]->Value;
                if(value.vt==3||value.vt==14)
                {
                    CString strTrans;
                    strTrans.Format("%ld",value.intVal);
                    Result.Add(strTrans);
                }
                else
                    if(value.vt==7)
                    {
                        COleDateTime time = value.date;
                        CString strTemp;
                        strTemp.Format("%d-%d-%d %s",time.GetYear(),time.GetMonth(),time.GetDay(),time.Format("%H:%M:%S"));
                        Result.Add(strTemp);
                    }
                    else
					{
						CString strTemp(value.bstrVal);
						Result.Add( strTemp );
					}
            }
            m_pRecordset->MoveNext();
        }
        m_pRecordset->Close();
        return TRUE;
    }
    else
    {
        m_pRecordset->Close();
        return FALSE;
    }
}

//�����ű�
BOOL CPData::OpenTable(CString strTable)
{
    try
    {
        m_nResultCol=0;
        m_nResultRow=0;
        m_pRecordset->CursorLocation=adUseClient;    //�����α�λ��,����Ϊ�ͻ�����ʽ,����GetRecordCount()����ֵ����
        m_pRecordset->Open(_variant_t(strTable),_variant_t((IDispatch *)m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdTable);
        m_nResultCol = m_pRecordset->Fields->GetCount();//�õ���ѯ���������
        m_nResultRow = m_pRecordset->GetRecordCount();  //�õ���ѯ���������
    }
    catch(_com_error&e)
    {
        AfxMessageBox(e.Description()+"E");
        return FALSE;
    }
    return TRUE;
}

BOOL CPData::OpenTable(CString strTable,CStringArray& Result, int& nCol)
{
    if(OpenTable(strTable)!=0)
    {
        Result.RemoveAll();
        _variant_t value;
        for(int i=0;i<m_nResultRow;i++)
        {
            for(int j=0;j<m_nResultCol;j++)
            {
                value=m_pRecordset->Fields->Item[(long)(/*i*m_nResultCol+*/j)]->Value;
                if(value.vt==3||value.vt==14)
                {
                    CString strTrans;
                    strTrans.Format("%ld",value.intVal);
                    Result.Add(strTrans);
                }
                else
                    if(value.vt==7)
                    {
                        COleDateTime time = value.date;
                        CString strTemp;
                        strTemp.Format("%d-%d-%d %s",time.GetYear(),time.GetMonth(),time.GetDay(),time.Format("%H:%M:%S"));
                        Result.Add(strTemp);
                    }
                    else
					{
						CString strTemp(value.bstrVal);
						Result.Add( strTemp );
					}
            }
            m_pRecordset->MoveNext();
        }
        m_pRecordset->Close();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/////////////////////////////////////////////////////////////////////////////
///�رս����
/////////////////////////////////////////////////////////////////////////////
void CPData::CloseRecordset()
{
    if(m_pRecordset->State != adStateClosed)
        m_pRecordset->Close();
}
BOOL CPData::ExecuteTrans(CStringArray& aSql)
{
    try{
        INT_PTR nNum = aSql.GetSize();
        m_pConnection->BeginTrans();
        for(int i=0;i<nNum;i++)
        {
            CString strSql = aSql.GetAt(i);
            m_pConnection->Execute((_bstr_t)aSql.GetAt(i),NULL,adCmdText);
        }
        m_pConnection->CommitTrans();
        return TRUE;
    }
    catch(_com_error& e)
    {
        m_pConnection->RollbackTrans();
        AfxMessageBox(e.Description()+"F");
        return FALSE;
    }
}
