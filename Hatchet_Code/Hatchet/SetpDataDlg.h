#pragma once
#include "afxwin.h"


// CSetpDataDlg �Ի���

class CSetpDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetpDataDlg)

public:
	CSetpDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetpDataDlg();

// �Ի�������
	enum { IDD = IDD_SETPDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboDemo();
	DECLARE_MESSAGE_MAP()

	CComboBox m_com_demo;

public:
	CString			szSiteUrl;							//Shell Url
	int				nSiteScript;						//Shell �ű�����
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

};
