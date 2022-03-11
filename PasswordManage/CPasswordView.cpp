#include "CPasswordView.h"
#include "PasswordManage.h"
#include <afxcontrolbars.h>


extern CTabView* g_pTabView;

IMPLEMENT_DYNCREATE(CPasswordView, CListView)

CPasswordView::CPasswordView()
{
	m_pListCtrl = NULL;
}

CPasswordView::~CPasswordView()
{

}

BEGIN_MESSAGE_MAP(CPasswordView, CListView)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_WINDOWPOSCHANGING()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDoubleclick)
END_MESSAGE_MAP()

void CPasswordView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

BOOL CPasswordView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

void CPasswordView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	m_pListCtrl = &GetListCtrl();
	m_pListCtrl->SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT /*| LVS_EX_FLATSB*/ |
		LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);
	m_pListCtrl->InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50);
	m_pListCtrl->InsertColumn(1, _T("名称"), LVCFMT_LEFT, 100);
	m_pListCtrl->InsertColumn(2, _T("用户名"), LVCFMT_LEFT, 100);
	m_pListCtrl->InsertColumn(3, _T("密码"), LVCFMT_LEFT, 200);
	m_pListCtrl->InsertColumn(4, _T("URL"), LVCFMT_LEFT, 200);
	m_pListCtrl->InsertColumn(5, _T("备注"), LVCFMT_LEFT, 200);

}

void CPasswordView::OnSize(UINT nType, int cx, int cy)
{
	CListView::OnSize(nType, cx, cy);
	
}

BOOL CPasswordView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{

	return CListView::OnNotify(wParam, lParam, pResult);
}

void CPasswordView::LoadPasswordInfo()
{
	std::vector<PasswordColumnInfo*> vecPasswordInfo;
	SqliteDatabase::GetDBController().GetPasswordInfoList(vecPasswordInfo);
	for each (auto info in vecPasswordInfo)
	{
		BOOL bFind = false;
		int nTabs = g_pTabView->GetTabControl().GetTabsNum();
		for (int nTabIndex = 0; nTabIndex < nTabs; nTabIndex++)
		{
			CString strTmpGroupName;
			g_pTabView->GetTabControl().GetTabLabel(nTabIndex, strTmpGroupName);
			if (strTmpGroupName == info->GroupName.c_str())
			{
				bFind = true;
				CPasswordView* pView = DYNAMIC_DOWNCAST(CPasswordView, g_pTabView->GetTabControl().GetTabWnd(nTabIndex));
				pView->InsertListInfo(info);
			}
		}
		if (!bFind)
		{
			g_pTabView->AddView(RUNTIME_CLASS(CPasswordView), info->GroupName.c_str());
			CPasswordView* pView = DYNAMIC_DOWNCAST(CPasswordView, g_pTabView->GetTabControl().GetTabWnd(nTabs));
			pView->OnInitialUpdate();
			pView->InsertListInfo(info);
		}
	}
}

void  CPasswordView::ShowList(std::vector<PasswordColumnInfo*> vectPasswordInfoList)
{
	m_pListCtrl->DeleteAllItems();
	
	for each (auto info in vectPasswordInfoList)
	{
		int nCnt = m_pListCtrl->GetItemCount();
		int index = m_pListCtrl->InsertItem(nCnt, "", 0);
		m_pListCtrl->SetItemText(index, 0, std::to_string(index+1).c_str());
		m_pListCtrl->SetItemText(index, 1, info->Name.c_str());
		m_pListCtrl->SetItemText(index, 2, info->Username.c_str());
		m_pListCtrl->SetItemText(index, 3, _T("******"));
		m_pListCtrl->SetItemText(index, 4, info->Url.c_str());
		m_pListCtrl->SetItemText(index, 5, info->Notes.c_str());
		m_pListCtrl->SetItemData(index, (DWORD)info);
	}
}

void CPasswordView::InsertListInfo(PasswordColumnInfo* info)
{
	int nCnt = m_pListCtrl->GetItemCount();
	int index = m_pListCtrl->InsertItem(nCnt, "", 0);
	m_pListCtrl->SetItemText(index, 0, std::to_string(index + 1).c_str());
	m_pListCtrl->SetItemText(index, 1, info->Name.c_str());
	m_pListCtrl->SetItemText(index, 2, info->Username.c_str());
	m_pListCtrl->SetItemText(index, 3, _T("******"));
	m_pListCtrl->SetItemText(index, 4, info->Url.c_str());
	m_pListCtrl->SetItemText(index, 5, info->Notes.c_str());
	m_pListCtrl->SetItemData(index, (DWORD)info);
}

void CPasswordView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{

}

void CPasswordView::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	CListView::OnWindowPosChanging(lpwndpos);

	ShowScrollBar(SB_HORZ, FALSE);
	ModifyStyle(WS_HSCROLL, 0, SWP_DRAWFRAME);
}

void CPasswordView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CListView::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_LEFT || nChar == VK_RIGHT)
	{
		CMFCTabCtrl* pTabWnd = DYNAMIC_DOWNCAST(CMFCTabCtrl, GetParent());
		ASSERT_VALID(pTabWnd);

		pTabWnd->SynchronizeScrollBar();
	}
}

void CPasswordView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	PasswordColumnInfo* ptmpInfo = NULL;
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	int nIndex = m_pListCtrl->GetNextSelectedItem(pos);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_RIGHTCLICK);
	CMenu* pMenu;
	pMenu = menu.GetSubMenu(0);
	if (nIndex == -1)
	{
		pMenu->EnableMenuItem(ID_MENU_DELETE, MF_DISABLED);
		pMenu->EnableMenuItem(ID_MENU_EDIT, MF_DISABLED);
	}


	CPoint	p;
	GetCursorPos(&p);
	int nMenuResult = pMenu->TrackPopupMenu(TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);

	if (nIndex != -1 && (nMenuResult == ID_MENU_EDIT || nMenuResult == ID_MENU_DELETE))
		ptmpInfo = (PasswordColumnInfo*)m_pListCtrl->GetItemData(nIndex);
	CSetInfo setInfoDlg(ptmpInfo);
	std::vector<PasswordColumnInfo*> vecPasswordInfolList;
	int nTab = g_pTabView->GetTabControl().GetActiveTab();
	CPasswordView* pView = DYNAMIC_DOWNCAST(CPasswordView, g_pTabView->GetTabControl().GetTabWnd(nTab));
	switch (nMenuResult)
	{
	case ID_MENU_ADD:

		setInfoDlg.DoModal();
		break;
	case ID_MENU_DELETE:
		if (ptmpInfo != NULL)
		{
			ptmpInfo->Isdelete = 1;
			SqliteDatabase::GetDBController().RemovePasswordInfo(*ptmpInfo);
			SqliteDatabase::GetDBController().GetGroupListInfo(vecPasswordInfolList, ptmpInfo->GroupName);
			if (vecPasswordInfolList.size())
			{
				pView->ShowList(vecPasswordInfolList);
			}
			else
			{
				g_pTabView->RemoveView(nTab);
			}
		}
		break;
	case ID_MENU_EDIT:
		setInfoDlg.DoModal();
		break;
	default:
		break;
	}
}

void CPasswordView::OnDoubleclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	PasswordColumnInfo* ptmpInfo = NULL;
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	int nIndex = m_pListCtrl->GetNextSelectedItem(pos);
	if (nIndex == -1)
		return;
	ptmpInfo = (PasswordColumnInfo*)m_pListCtrl->GetItemData(nIndex);
	CSetInfo setInfoDlg(ptmpInfo);
	setInfoDlg.DoModal();
}