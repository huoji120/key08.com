#pragma once
#include "panel.h"

class CCaptionCenterPanel :
	public CPanel
{
public:
	CCaptionCenterPanel(void);
	~CCaptionCenterPanel(void);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	// ��Դλͼ
    CBitmap m_bmpRes;
    // ��Դ�豸
    CDC     ImageDC;
	afx_msg bool AddTab(CString szName, int nType);
	afx_msg void RemoveTab(int index);

protected:
	#define TB_CLR_TRAN     RGB(255, 0, 255)
	#define TB_MAX_TAB      30					//������

	HICON	hIcon_Dir;
	HICON	hIcon_Cmd;
	HICON	hIcon_Data;
	HICON	hIcon_FileSave;
	HICON	hIcon_Proxy;
	HICON	hIcon_EvalCode;
	HICON	hIcon_Registry;
	HICON	hIcon_Browse;

	int		nHover;						//����ǻ��ĸ���
	int		nOldHover;					//����ǻ��ĸ���
	int		TabLeng;					//��ǩ����

	CString m_svTabNames[TB_MAX_TAB];	// ��ǩ��
	DWORD   m_dwvTabDatas[TB_MAX_TAB];	// ��ǩ����
	int     m_iTabNum;					// ��ǩ��
	int		m_nType[TB_MAX_TAB];		// ͼ������ 1 2 3 4	
};

extern CCaptionCenterPanel*	pCaptionCenterPanel;