
// PasswordManageDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PasswordManage.h"
#include "PasswordManageDlg.h"
#include "afxdialogex.h"
#include "CSetInfoDlg.h"
#include "SqliteDatabase.h"
#include "CPasswordView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPasswordManageDlg 对话框



CPasswordManageDlg::CPasswordManageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PASSWORDMANAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPasswordManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_LIST_PWMANAGE, m_PasswordList);
	//DDX_Control(pDX, IDC_TAB_PASSWORD, m_PasswordTab);
}

BEGIN_MESSAGE_MAP(CPasswordManageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PWMANAGE, OnRclick)
END_MESSAGE_MAP()



// CPasswordManageDlg 消息处理程序

BOOL CPasswordManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(TRUE);

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_PasswordList.SetExtendedStyle(m_PasswordList.GetExtendedStyle() | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_PasswordList.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50);
	m_PasswordList.InsertColumn(1, _T("名称"), LVCFMT_LEFT, 100);
	m_PasswordList.InsertColumn(2, _T("用户名"), LVCFMT_LEFT, 100);
	m_PasswordList.InsertColumn(3, _T("密码"), LVCFMT_LEFT, 200);
	m_PasswordList.InsertColumn(4, _T("URL"), LVCFMT_LEFT, 200);
	m_PasswordList.InsertColumn(5, _T("备注"), LVCFMT_LEFT, 200);

	//ShowList();
	m_wndTabControl.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 100);
	AddGroup("11111");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPasswordManageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPasswordManageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPasswordManageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CPasswordManageDlg::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle)
{
	CCreateContext contextT;
	contextT.m_pCurrentDoc = NULL;
	contextT.m_pNewViewClass = pViewClass;
	contextT.m_pNewDocTemplate = NULL;

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
		CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL
		DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	dwStyle &= ~WS_BORDER;

	int nTab = m_wndTabControl.GetItemCount();
	CRect rect(0, 0, 0, 0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, &m_wndTabControl, (AFX_IDW_PANE_FIRST + nTab), &contextT))
	{
		TRACE0("Warning: couldn't create client tab for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}
	m_wndTabControl.InsertItem(nTab, lpszTitle);
	
	pWnd->SetOwner(this);

	return TRUE;
}

BOOL CPasswordManageDlg::AddGroup(LPCTSTR lpszTitle)
{
	BOOL Result = AddView(RUNTIME_CLASS(CPasswordView), lpszTitle);
	return Result;
}

void  CPasswordManageDlg::ShowList()
{
	m_PasswordList.DeleteAllItems();
	std::vector<PasswordColumnInfo*> vectPasswordInfoList;
	SqliteDatabase::GetDBController().GetPasswordInfoList(vectPasswordInfoList);
	for each (auto info in vectPasswordInfoList)
	{
		int nCnt = m_PasswordList.GetItemCount();
		int index = m_PasswordList.InsertItem(nCnt, "", 0);
		m_PasswordList.SetItemText(index, 0, std::to_string(index).c_str());
		m_PasswordList.SetItemText(index, 1, info->Name.c_str());
		m_PasswordList.SetItemText(index, 2, info->Username.c_str());
		m_PasswordList.SetItemText(index, 3, info->Password.c_str());
		m_PasswordList.SetItemText(index, 4, info->Url.c_str());
		m_PasswordList.SetItemText(index, 5, info->Notes.c_str());
		m_PasswordList.SetItemData(index, (DWORD)info);
	}
}


void CPasswordManageDlg::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	PasswordColumnInfo* ptmpInfo = NULL;
	POSITION pos = m_PasswordList.GetFirstSelectedItemPosition();
	int nIndex = m_PasswordList.GetNextSelectedItem(pos);
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

	if(nIndex != -1 && (nMenuResult == ID_MENU_EDIT || nMenuResult == ID_MENU_DELETE))
		ptmpInfo = (PasswordColumnInfo*)m_PasswordList.GetItemData(nIndex);
	CSetInfo setInfoDlg(ptmpInfo);
	switch (nMenuResult)
	{
	case ID_MENU_ADD:

		setInfoDlg.DoModal();
		ShowList();
		break;
	case ID_MENU_DELETE:
		if (ptmpInfo != NULL)
		{
			ptmpInfo->Isdelete = 1;
			SqliteDatabase::GetDBController().RemovePasswordInfo(*ptmpInfo);
		}
		ShowList();
		break;
	case ID_MENU_EDIT:
		setInfoDlg.DoModal();
		ShowList();
		break;
	default:
		break;
	}
}

