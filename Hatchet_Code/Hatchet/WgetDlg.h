#pragma once


// CWgetDlg �Ի���

class CWgetDlg : public CDialog
{
	DECLARE_DYNAMIC(CWgetDlg)

public:
	CWgetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWgetDlg();

// �Ի�������
	enum { IDD = IDD_WGET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	CString szWgetPath;
	afx_msg void OnBnClickedOnWget();
	afx_msg void OnEnSetfocusEditWgetUrl();


	CString			szSiteUrl;							//Shell Url
	CString			szSitePass;							//Shell Pass
	int				nSiteCode;							//Shell Code ����
	int				nSiteScript;						//Shell �ű�����

	bool			bData;								//�����·�������룬�Ͳ�����֮�ı���
};
