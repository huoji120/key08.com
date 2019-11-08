#pragma once


// CMainDlg �Ի���

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMRClickSitelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditTypetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkSitelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTypetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTypetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTypetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnColumnclickSitelist(NMHDR *pNMHDR, LRESULT *pResult);//����

public:
	bool ShowSiteList(CString szTypeName=""/*��������*/, CString szFind=""/*���ҹؼ���*/, CString szSort="NewTime"/*�ĸ���*/, CString szDesc_Asc="Desc");
	afx_msg void OnMainSiteAdd();			//���
	afx_msg void OnMain2SiteAdd();			//���

	afx_msg void OnMainSiteEdit();			//�༭
	afx_msg void OnMainSiteDelete();		//ɾ��
	afx_msg void OnMainCopyUrl();			//����վ��
	afx_msg void OnMainSiteFind();			//����
	afx_msg void OnMainSite2Find();			//����	

	afx_msg void OnMainUpdateIni(bool bNoCreat=true);			//����INI
	afx_msg void OnMainSiteUpdateIni();		//����INI
	afx_msg void OnMainSite2UpdateIni();	//����INI
	afx_msg void OnMainUpdatePost();		//��������POST����

	afx_msg void OnHatchetTypeAdd();
	afx_msg void OnHatchetTypeEdit();
	afx_msg void OnHatchetTypeDel();

	afx_msg void AddFileTab();
	afx_msg void AddCmdTab();
	afx_msg void AddDataTab();
	afx_msg void AddProxyTab();
	afx_msg void AddEvalCodeTab();
	afx_msg void AddRegistryTab();
	afx_msg void AddBrowseTab();

protected:
	HTREEITEM	select_ht;				//�ڽ����Tree ��� �༭ ɾ�� ��Ҫ�õ�
	int			nHatchetTypeAdd_Edit;	//ʧȥ�����ʱ���жϵ���Ҫ��Ҫ 1=add 2=edit
	CString		szSelect_Type;			//���shell��ʱ��Ĭ����ʾ��ǰ����

	CString		szSort_TypeName/*��������*/,szSort_Find/*���ҹؼ���*/,szSort_Sort/*�ĸ���*/,szSort_Desc_Asc;//���򣬼��䡣
public:
	CListCtrl	m_sitelist;
	CEdit		m_Edit_Find;
	CTreeCtrl	m_typetree;
	CImageList	m_iImageList;						//ͼ����
	CImageList	m_iImageList_Type;					//ͼ����

};
