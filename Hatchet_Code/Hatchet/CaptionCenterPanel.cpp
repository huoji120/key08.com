#include "StdAfx.h"
#include "CaptionCenterPanel.h"
#include "MainHatchet.h"
#include "resource.h"


CCaptionCenterPanel::CCaptionCenterPanel(void)
{
	TabLeng = 4+140+9;		//ÿһ������
	m_iTabNum = 0;			// ��ǩ��
	nShowTab = -1;			//��ǰ��ʾ��TAB���Աȣ�����ÿ�ζ��䣬�ŵ�ȫ�֣���Ϊ�����ߣ�û�иı�
	nHover = -1;
}

CCaptionCenterPanel::~CCaptionCenterPanel(void)
{
}

CCaptionCenterPanel* pCaptionCenterPanel;


BEGIN_MESSAGE_MAP(CCaptionCenterPanel, CPanel)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)	//�뿪
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CCaptionCenterPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x > (TabLeng * m_iTabNum))
	{
		CMainHatchet* pMainHatchet = (CMainHatchet*)AfxGetMainWnd();
		if (pMainHatchet)
		{
			pMainHatchet->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
	}

	CPanel::OnLButtonDown(nFlags, point);
}

void CCaptionCenterPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (point.x > (TabLeng * m_iTabNum))
	{
		CMainHatchet* pMainHatchet = (CMainHatchet*)AfxGetMainWnd();
		if (pMainHatchet)
		{
			pMainHatchet->SendMessage(WM_NCLBUTTONDBLCLK, HTCAPTION, 0);
		}
	}
	CPanel::OnLButtonDblClk(nFlags, point);
}



int CCaptionCenterPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bmpRes.LoadBitmap(IDB_TAB);

	hIcon_Dir = AfxGetApp()->LoadIcon(IDI_DIR);
	hIcon_Cmd = AfxGetApp()->LoadIcon(IDI_CMD);
	hIcon_Data = AfxGetApp()->LoadIcon(IDI_DATA);
	hIcon_FileSave = AfxGetApp()->LoadIcon(IDI_FILESAVE);
	hIcon_Proxy = AfxGetApp()->LoadIcon(IDI_PROXY);
	hIcon_EvalCode = AfxGetApp()->LoadIcon(IDI_TYPE);
	hIcon_Registry = AfxGetApp()->LoadIcon(IDI_REGISTRY);
	hIcon_Browse = AfxGetApp()->LoadIcon(IDI_BROWSE);

	pCaptionCenterPanel = (CCaptionCenterPanel*)this;		//��ֵ��ȫ�֣����TABʱ�õ�

	return 0;
}


// ��ӱ�ǩ 1 File 2 Cmd 3 Data 4 SaveFile 5 Proxy 6 EvalCode 7 Registry 8 Browse
bool CCaptionCenterPanel::AddTab(CString szName, int nType)
{
	if(m_iTabNum < TB_MAX_TAB)
	{
		nShowTab = m_iTabNum;
		m_nType[m_iTabNum] = nType;
		m_svTabNames[m_iTabNum]  = szName;
		m_iTabNum++;
		//////////////////////////////////////////////////////////////////////////		����ÿһ���ĳ��ȣ�ƽ������
		CRect rcClient;
		GetClientRect(rcClient);
		if (m_iTabNum == 0)
		{
			TabLeng = 4+140+9;		//ÿһ������
		}else{
			int nAverage = rcClient.Width() / m_iTabNum;
			if (nAverage >= 4+140+9)
			{
				TabLeng = 4+140+9;		//ÿһ������
			}else{
				TabLeng = nAverage;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		Invalidate();
		return true;
	}
	return false;
}

// ɾ����ǩ
void CCaptionCenterPanel::RemoveTab(int index)
{
	CRect rcClient;
	GetClientRect(rcClient);

	if (m_iTabNum == 1)//һ����һ�������Լ�ɾ���Ϳ�����
	{
		m_nType[index]  = 0;
		m_svTabNames[index]  = "";
		m_iTabNum--;
		//////////////////////////////////////////////////////////////////////////		����ÿһ���ĳ��ȣ�ƽ������
		if (m_iTabNum == 0)
		{
			TabLeng = 4+140+9;		//ÿһ������
		}else{
			int nAverage = rcClient.Width() / m_iTabNum;
			if (nAverage >= 4+140+9)
			{
				TabLeng = 4+140+9;		//ÿһ������
			}else{
				TabLeng = nAverage;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		Invalidate();
	}
	else if (m_iTabNum > 1)//��ֹһ����Ҫ�Ѻ���ģ�����ǰ����
	{
		for(;index < m_iTabNum - 1; index++)
		{
			m_nType[index]  = m_nType[index + 1];
			m_svTabNames[index]  = m_svTabNames[index + 1];
		}
		m_iTabNum--;
		//////////////////////////////////////////////////////////////////////////		����ÿһ���ĳ��ȣ�ƽ������
		if (m_iTabNum == 0)
		{
			TabLeng = 4+140+9;		//ÿһ������
		}else{
			int nAverage = rcClient.Width() / m_iTabNum;
			if (nAverage >= 4+140+9)
			{
				TabLeng = 4+140+9;		//ÿһ������
			}else{
				TabLeng = nAverage;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		Invalidate();
		nShowTab = m_iTabNum;
	}
	return;
}


void CCaptionCenterPanel::OnPaint()
{
	CPaintDC dc(this);

	if ( ImageDC.m_hDC == NULL)
	{
		ImageDC.CreateCompatibleDC(&dc);
		ImageDC.SelectObject(&m_bmpRes);
	}

	CString szText;
	CRect rcText;
	//l��t��r��b
	//��ߡ��ϱߡ��ұߺ͵ױ�
	rcText.top = 4;
	rcText.bottom = 21;


	for (int i=0; i<m_iTabNum; i++)
	{
		/*
		dc.TransparentBlt(TabLeng*i, 0, 4, 21, &ImageDC, 0, 11, 4, 21, TB_CLR_TRAN);				//ǰ�� 4�����
		dc.TransparentBlt(TabLeng*i + 4, 0, (TabLeng-4-9), 21, &ImageDC, 4, 11, 1, 21, TB_CLR_TRAN);			//�м� 100
		dc.TransparentBlt(TabLeng*i + (TabLeng-9), 0, 9, 21, &ImageDC, 5 , 11, 9, 21, TB_CLR_TRAN);		//���� 9
		*/
		if (nShowTab != i)
		{
			dc.TransparentBlt(TabLeng*i, 0, 4, 21, &ImageDC, 15, 11, 4, 21, TB_CLR_TRAN);						//ǰ�� 4�����
			dc.TransparentBlt(TabLeng*i + 4, 0, (TabLeng-4-9), 21, &ImageDC, 19, 11, 1, 21, TB_CLR_TRAN);		//�м� 100
			dc.TransparentBlt(TabLeng*i + (TabLeng-9), 0, 9, 21, &ImageDC, 20 , 11, 9, 21, TB_CLR_TRAN);		//���� 9
		}else{
			dc.TransparentBlt(TabLeng*i, 0, 4, 21, &ImageDC, 0, 11, 4, 21, TB_CLR_TRAN);						//ǰ�� 4�����
			dc.TransparentBlt(TabLeng*i + 4, 0, (TabLeng-4-9), 21, &ImageDC, 4, 11, 1, 21, TB_CLR_TRAN);		//�м� 100
			dc.TransparentBlt(TabLeng*i + (TabLeng-9), 0, 9, 21, &ImageDC, 5 , 11, 9, 21, TB_CLR_TRAN);			//���� 9
		}


		if (m_nType[i] == 1)
		{
			DrawIconEx(dc.m_hDC, TabLeng*i + 5, 4, hIcon_Dir, 16, 16, 0, 0, DI_NORMAL);					//����ͼ��
		}else if (m_nType[i] == 2)
		{
			DrawIconEx(dc.m_hDC, TabLeng*i + 5, 4, hIcon_Cmd, 16, 16, 0, 0, DI_NORMAL);					//����ͼ��
		}else if (m_nType[i] == 3)
		{
			DrawIconEx(dc.m_hDC, TabLeng*i + 5, 4, hIcon_Data, 16, 16, 0, 0, DI_NORMAL);					//����ͼ��
		}else if (m_nType[i] == 4)
		{
			DrawIconEx(dc.m_hDC, TabLeng*i + 5, 4, hIcon_FileSave, 16, 16, 0, 0, DI_NORMAL);					//����ͼ��
		}else if (m_nType[i] == 5)
		{
			DrawIconEx(dc.m_hDC, TabLeng*i + 5, 4, hIcon_Proxy, 16, 16, 0, 0, DI_NORMAL);					//����ͼ��
		}else if (m_nType[i] == 6)
		{
			DrawIconEx(dc.m_hDC, TabLeng*i + 5, 4, hIcon_EvalCode, 16, 16, 0, 0, DI_NORMAL);					//����ͼ��
		}else if (m_nType[i] == 7)
		{
			DrawIconEx(dc.m_hDC, TabLeng*i + 5, 4, hIcon_Registry, 16, 16, 0, 0, DI_NORMAL);					//����ͼ��
		}else if (m_nType[i] == 8)
		{
			DrawIconEx(dc.m_hDC, TabLeng*i + 5, 4, hIcon_Browse, 16, 16, 0, 0, DI_NORMAL);					//����ͼ��
		}else{
			DrawIconEx(dc.m_hDC, TabLeng*i + 5, 4, hIcon_FileSave, 16, 16, 0, 0, DI_NORMAL);					//����ͼ��
		}

		rcText.left = TabLeng*i + 4 + 16;
		rcText.right = rcText.left + (TabLeng-4-16);

		szText = m_svTabNames[i];
		int nOldBkMode = dc.SetBkMode(TRANSPARENT);
		dc.DrawText(szText, szText.GetLength(), rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_WORD_ELLIPSIS);
		dc.SetBkMode(nOldBkMode);

		if (nHover == i)
		{
			dc.TransparentBlt(TabLeng*(i+1) -4 -9 -6, 5, 11, 11, &ImageDC, 11, 0, 11, 11, TB_CLR_TRAN);	//�ر� X
		}else{
			dc.TransparentBlt(TabLeng*(i+1) -4 -9 -6, 5, 11, 11, &ImageDC, 0, 0, 11, 11, TB_CLR_TRAN);	//�ر� X
		}
	}
}

//�ر� X ����
void CCaptionCenterPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1;
	::TrackMouseEvent(&tme);

	int PointX = point.x;
	if (PointX < TabLeng )
	{
		if ( PointX > (TabLeng -4 -9 -6) && PointX < (TabLeng -9) )
		{
			nHover = 0;
			if (nHover != nOldHover)
			{
				nOldHover = nHover;
				Invalidate();
			}
		}else{
			nHover = -1;
			if (nHover != nOldHover)
			{
				nOldHover = nHover;
				Invalidate();
			}
		}
	}else{
		int nRange = (int)PointX / TabLeng;
		nRange++;
		if (nRange > m_iTabNum)
		{
			CPanel::OnMouseMove(nFlags, point);
			return;
		}

		if ( PointX > (TabLeng*nRange -4 -9 -6) && PointX < (TabLeng*nRange -9) )
		{
			nHover = nRange-1;//�����ǻ��ĸ���

			if (nHover != nOldHover)
			{
				nOldHover = nHover;
				Invalidate();
			}
		}else{
			nHover = -1;
			if (nHover != nOldHover)
			{
				nOldHover = nHover;
				Invalidate();
			}
		}
	}
	CPanel::OnMouseMove(nFlags, point);
}

LRESULT CCaptionCenterPanel::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	nHover = -1;
	if (nHover != nOldHover)
	{
		nOldHover = nHover;
		Invalidate();
	}
	return TRUE;
}


void CCaptionCenterPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (point.x > (TabLeng * m_iTabNum))
	{
		CPanel::OnLButtonUp(nFlags, point);
		return;
	}

	int PointX = point.x;

	CMainHatchet* pMainHatchet = (CMainHatchet*)AfxGetMainWnd();
	if (!pMainHatchet)
	{
		CPanel::OnLButtonUp(nFlags, point);
		return;
	}

	if ( PointX < TabLeng )
	{
		if ( PointX > (TabLeng -4 -9 -6) && PointX < (TabLeng -9) )
		{
			nHover = -1;
			RemoveTab(0);
			pMainHatchet->DeleteTab(0);
		}else{
			if (nShowTab != 0)		//CCaptionLeftû�иı䣬���в����
			{
				nShowTab = 0;
				pMainHatchet->SelchangeTab(0);
				Invalidate();
			}
		}

	}else{
		int nRange = (int)PointX / TabLeng;
		nRange++;
		if ( PointX > (TabLeng*nRange -4 -9 -6) && PointX < (TabLeng*nRange -9) )
		{
			nHover = nRange-1;//�����ǻ��ĸ���
			RemoveTab(nHover);
			pMainHatchet->DeleteTab(nHover);
		}else{
			nHover = nRange-1;//�����ǻ��ĸ���
			if (nShowTab != nHover)
			{
				nShowTab = nHover;
				pMainHatchet->SelchangeTab( nHover );
				Invalidate();							//������ı�TAB��Ҫ����һ�£���Ȼû��
			}
		}
	}

	CPanel::OnLButtonUp(nFlags, point);
}


void CCaptionCenterPanel::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	//////////////////////////////////////////////////////////////////////////		����ÿһ���ĳ��ȣ�ƽ������
	CRect rcClient;
	GetClientRect(rcClient);
	if (m_iTabNum == 0)
	{
		TabLeng = 4+140+9;		//ÿһ������
	}else{
		int nAverage = rcClient.Width() / m_iTabNum;
		if (nAverage >= 4+140+9)
		{
			TabLeng = 4+140+9;		//ÿһ������
		}else{
			TabLeng = nAverage;
		}
	}
	//////////////////////////////////////////////////////////////////////////
}
