#pragma once


// CCmdDlg �Ի���

class CCmdDlg : public CDialog
{
	DECLARE_DYNAMIC(CCmdDlg)

public:
	CCmdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCmdDlg();

// �Ի�������
	enum { IDD = IDD_CMD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

//protected:
public:
//	afx_msg void GetCmdData(CString szCmd);

	CEdit			m_cmd;
	CWnd			*pEdit;								//�ı�����
	int				CharLen_Old;						//�س�ǰ�ĸ���
	int				CharLen_New;						//���µĸ���
	CString			Cmdtemp;
	CString			szCmdRoot;
	bool			bIsSetSel;							//�ᱻȫѡ
	CStringArray	szCmdDos;							//��ʷ����
	int				nCmdDosCont;						//���������¼�ͷ
	CString			OldCmdtemp;							//����������������Ϣ��һ�����������ʱ����Ҫ��
	bool			IsWin;								//�Ƿ�WINϵͳ (/) ���� (\)
	CString			WWWRoot;							//���漼������ʱ��û�У����ˣ���Ҫ�޸�
	CString			szSystemInfo;						//[C:D:E:F:G:X:Y:	Windows NT QN 6.1 build 7601 (Windows 7 Ultimate Edition Service Pack 1) i586(SYSTEM)]
	CString			szSiteUrl;							//Shell Url
	CString			szSiteJoinUrl;
	CString			szSiteJoinPass;
	CString			szSitePass;							//Shell Pass
	int				nSiteCode;							//Shell Code ����
	CString			szSiteCode;							//JSP ����
	CString			szSiteConfig;						//Shell Config
	int				nSiteScript;						//Shell �ű�����
	CString			szCmdFamatPageCode;					//������ʽ��TEMP

	CString			szCacheItem;						//���漼��
	CString			szCmdExeAddr;						//CMDλ��

	bool			bThreadFinish;						//�ж��Ƿ���������ִ�����
	CString			szCmd;								//�߳��Ҫִ�е�CMD
public:
	afx_msg void OnEnKillfocusCmd();
};
