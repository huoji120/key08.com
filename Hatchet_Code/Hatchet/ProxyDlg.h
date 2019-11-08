#pragma once
#include "afxwin.h"


// CProxyDlg �Ի���

class CProxyDlg : public CDialog
{
	DECLARE_DYNAMIC(CProxyDlg)

public:
	CProxyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProxyDlg();

// �Ի�������
	enum { IDD = IDD_PROXY };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void AddLog(CString strMsg);

protected:
	afx_msg void OnBnClickedButtonProxyStart();
	afx_msg void OnBnClickedButtonProxyStop();


protected:
	CString			szSiteUrl;							//Shell Url
	CString			szSitePass;							//Shell Pass
	int				nSiteCode;							//Shell Code ����
	int				nSiteScript;						//Shell �ű�����

	CListBox m_proxy_log;
	HANDLE hThread;	
};
