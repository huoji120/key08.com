#pragma once
#include "explorer1.h"
#include "afxwin.h"


// CBrowseDlg �Ի���

class CBrowseDlg : public CDialog
{
	DECLARE_DYNAMIC(CBrowseDlg)

public:
	CBrowseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBrowseDlg();

// �Ի�������
	enum { IDD = IDD_BROWSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBrowseOk();


public:
	CExplorer1 m_explorer;
	CComboBox m_com_browse;

public:
	DECLARE_EVENTSINK_MAP()
public:
	void StatusTextChangeExplorer1(LPCTSTR Text);
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
};
