#pragma once
#include "afxwin.h"


// CFileDlg �Ի���

class CFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileDlg();

// �Ի�������
	enum { IDD = IDD_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg bool GetServerInfo();
	afx_msg void OnFileClearCache();			//��ջ���
	afx_msg void OnFileNewDirectory();			//�½�Ŀ¼
	afx_msg void OnFileNewFile();				//�½��ļ�
	afx_msg void OnFileRename();				//������
	afx_msg void OnFileChangeTime();			//����ʱ��
	afx_msg void OnFileEditFile();				//�༭�ļ�
	afx_msg void OnFileDeleteFiles();			//ɾ���ļ�
	afx_msg void OnFileWget();					//WGET
	afx_msg void OnFileUploadFile();			//�ϴ��ļ�
	afx_msg void OnFileDownloadFile();			//�����ļ�

	afx_msg void OnFileCut();					//�ļ�����
	afx_msg void OnFileCopy();					//�ļ�����
	afx_msg void OnFilePaste();					//�ļ�ճ��

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedReadfiles();
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickFilelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillFocusSubItemEdit();
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//afx_msg void OnTvnSelchangedFileTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangingFileTree(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileRefresh();				//ˢ��

	CListCtrl		m_filelist;
	CTreeCtrl		m_dirtree;
	CEdit			m_SubItemEdit;
	CComboBox		m_com_file_path;


	CImageList		m_iImageTree;				//ͼ����
	CImageList		m_iImageList;				//ͼ����
	CStringArray	szFilesArrCont;				//�ļ�����ͼ����� FilesIco �ĵڼ���
	HTREEITEM		m_hNowItem;					//�������ܣ���Ȼÿ�ζ�Ҫѭ������
	bool			IsWin;						//�Ƿ�WINϵͳ (/) ���� (\)
	CString			WWWRoot;					//���漼������ʱ��û�У����ˣ���Ҫ�޸�

	CString			szSystemInfo;						//Windows NT MY-785137BA5775 5.2 build 3790 (Windows Server 2003 Enterprise Edition Service Pack 2) i586(SYSTEM)
														//Linux mail.baidu.com 2.6.18-194.el5 #1 SMP Fri Apr 2 14:58:35 EDT 2010 i686(apache)
	CString			szSiteUrl;							//Shell Url
	CString			szSitePass;							//Shell Pass
	int				nSiteCode;							//Shell Code ����
	CString			szSiteCode;							//JSP ����
	CString			szSiteConfig;						//Shell Config
	int				nSiteScript;						//Shell �ű�����

	CString			szFileFamatPageCode;				//��ʽ��TEMP
	CString			szCacheItem;						//���漼��

	int				nSelectList;						//�͵ر༭��Ҫ�õ�

	CString			szFileCutName;						//�ļ����� �Է�Ϊ���ж�
	CString			szFileCutPath;						//�ļ����� �Է�Ϊ���ж�
	CString			szFileCopyName;						//�ļ����� �Է�Ϊ���ж�
	CString			szFileCopyPath;						//�ļ����� �Է�Ϊ���ж�

	bool			bThreadFinish;						//�ж��Ƿ���������ִ�����

	CString			GetFiles_szPath;
	bool			GetFiles_bNoDB;// = false
public:
	afx_msg void OnBnClickedUpperstory();
};
