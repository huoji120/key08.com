#pragma once
#include "afxwin.h"


// CFileSaveDlg �Ի���

class CFileSaveDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileSaveDlg)

public:
	CFileSaveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileSaveDlg();

// �Ի�������
	enum { IDD = IDD_FILESAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnGetContent();
	afx_msg void OnSave();

//	afx_msg bool GetContent(CString szFilePath);

	afx_msg void OnEnSetfocusEditFilepath();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	CEdit	m_FileSave_Edit_Find;
	CString szSiteUrl;
	CString szSitePass;
	CString szSiteFileSave;
	int		nSiteScript;
	int		nSiteCode;
	bool	IsWin;								//�Ƿ�WINϵͳ (/) ���� (\)
	int		nFindPos;							//���ҵĿ�ʼλ��
	CString szFindStr;							//���ҵ��ı������ݿؼ����Ҫ�Ƚ�

	bool			bThreadFinish;						//�ж��Ƿ���������ִ�����
	CString			szFilePath;							//�ļ�·��
};
