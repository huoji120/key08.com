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
#include "StdAfx.h"
/*****************************************************************
CPFile.h
******************************************************************/
class CPData  
{
public:
    
    //Ĭ�ϳ�ʼ�����캯��
    CPData();
    
    //�������ӵĹ��캯��
    CPData(_ConnectionPtr pConnection);
    
    //��������
    virtual ~CPData();
public:
    
	BOOL Create(CString strFileName);

    //�������ݿ�
	BOOL Connect(CString strFileName);
    
    //�ر����ݿ������
    void DisConnect();
    
	//�ж��ֶδ���
	bool CheckFields(CString strSql);

    //���ݿ��ѯ��䣬���������ݿ�ĸ����ֶν��в�ѯ
    //����ɹ�����TRUE,���򷵻�FALSE.��ѯ�Ľ���洢����Ĺ��г�Ա����m_pRecordset��
    //��ѯ����������������ֱ𱣴��ڹ��г�Ա����m_nResultRow��m_nResultCol��
    int Select(CString strSql);
    
    //��ѯ��䣬����Խ�����ѯһ���ֶε�������д���
    //��������CStringArray���͵ı���pResult��
    int Select(CString strSql,CStringArray& Result);
    
    //�Զ���ֶν��в�ѯ
    BOOL SelectMulitCol(CString strSql,CStringArray& Result);
    
    //�򿪱�
    BOOL OpenTable(CString strTable);
    
    //�򿪱�
    BOOL OpenTable(CString strTable,CStringArray& Result, int& nCol);
    
    //���������ĸ��²���
    BOOL Execute(CString strSql);
public:
    BOOL ExecuteTrans(CStringArray& aSql);    
    
    //�رս������
    void CloseRecordset();
    
    //�õ��������������
    long GetResultCol();
    
    //�õ��������������
    long GetResultRow();
    
    //�õ��������
    _RecordsetPtr GetResult();
private:
    
    //���ݿ�������صĽ������
    long m_nResultRow;
    
    //���ص�_RecordsetPtr������
    long m_nResultCol;
    
    //����ָ��
    _ConnectionPtr m_pConnection;
    
    //����ִ��ָ��
    _CommandPtr m_pCommand;
    
    //�����ָ��
    _RecordsetPtr m_pRecordset;
};