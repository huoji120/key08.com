#pragma once
#include "afxwin.h"


// CDataDlg �Ի���

class CDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataDlg)

public:
	CDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataDlg();

// �Ի�������
	enum { IDD = IDD_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonExecsql();
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillFocusSubItemEdit();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnDataGetcount();			//��ȡ��ĸ���
	afx_msg void OnDataGet20Lines();		//��ȡ20������
	afx_msg void OnDataDropTable();			//ɾ����
	afx_msg void OnDataSetclipboard();
	afx_msg void OnDataExportHtml();		//����HTML
	afx_msg void OnDataExportText();		//����TEXT
	/*
	afx_msg void GetDataBases();
	afx_msg void GetTables(HTREEITEM hParent, CString szDataName);
	afx_msg void GetColumns(HTREEITEM hParent, CString szDataName, CString szTableName);
	afx_msg void GetExecSql(CString szSql);
	*/
	afx_msg void OnBnClickedDataConfig();

public:
	CString			sz_datahost,sz_datauser,sz_datapass;
	CImageList		m_iImageTree;						//ͼ����
	HTREEITEM		select_ht;							//�ڽ����Tree ��� �༭ ɾ�� ��Ҫ�õ�
	CStringArray	CtrlListHeaderArr;


	CString			szSystemInfo;						//Windows NT MY-785137BA5775 5.2 build 3790 (Windows Server 2003 Enterprise Edition Service Pack 2) i586(SYSTEM)
														//Linux mail.baidu.com 2.6.18-194.el5 #1 SMP Fri Apr 2 14:58:35 EDT 2010 i686(apache)
	CString			szSiteUrl;							//Shell Url
	CString			szSitePass;							//Shell Pass
	int				nSiteCode;							//Shell Code ����
	CString			szSiteCode;							//JSP	����
	CString			szSiteConfig;						//Shell Config
	int				nSiteScript;						//Shell �ű�����

	CTreeCtrl		m_datatree;
	CListCtrl		m_datalist;
	CComboBox		m_comDatas;
	CEdit			m_SubItemEdit;
	CComboBox		m_com_sql;

	bool			bThreadFinish;						//�ж��Ƿ���������ִ�����

	HTREEITEM		GetTables_hParent;
	CString			GetTables_szDataName;

	HTREEITEM		GetColumns_hParent;
	CString			GetColumns_szDataName;
	CString			GetColumns_szTableName;

	CString			GetExecSql_szSql;
};
