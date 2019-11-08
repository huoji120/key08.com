#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CRegistryDlg �Ի���

class CRegistryDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegistryDlg)

public:
	CRegistryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegistryDlg();

// �Ի�������
	enum { IDD = IDD_REGISTRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	CTreeCtrl m_registry_tree;
	CListCtrl m_registry_list;

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedRegistryButton();
	afx_msg void OnTvnSelchangingRegistryTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRegistryGet(CString szPath, bool bNoSelectTempDB = false);
	afx_msg void OnBnClickedRegistryRadioCmd();
	afx_msg void OnBnClickedRegistryRadioWscript();
	afx_msg void OnNMRclickRegistryList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRegistryRefresh();				//ˢ��
	afx_msg void OnRegistryClearCache();			//��ջ���

public:
	CString			szSiteUrl;							//Shell Url
	CString			szSitePass;							//Shell Pass
	int				nSiteCode;							//Shell Code ����
	int				nSiteScript;						//Shell �ű�����
	CComboBox		m_registry_com;
	bool			bThreadFinish;						//�ж��Ƿ���������ִ�����
	CString			RegistryGetData_szPath;
	bool			RegistryGetData_bNoDB;				//��Ҫ������	
};
