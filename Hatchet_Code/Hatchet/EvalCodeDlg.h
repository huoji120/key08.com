#pragma once


// CEvalCodeDlg �Ի���

class CEvalCodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CEvalCodeDlg)

public:
	CEvalCodeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEvalCodeDlg();

// �Ի�������
	enum { IDD = IDD_EVALCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedEvalcodeImport();
	afx_msg void OnBnClickedEvalcodeExport();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CString			szSiteUrl;							//Shell Url
	CString			szSitePass;							//Shell Pass
	int				nSiteCode;							//Shell Code ����
	int				nSiteScript;						//Shell �ű�����
	CString			szSiteJoinUrl;
	CString			szSiteJoinPass;

};
