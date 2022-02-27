
// PasswordManageView.cpp: CPasswordManageView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "PasswordManage.h"
#endif

#include "PasswordManageDoc.h"
#include "PasswordManageView.h"
#include "CPasswordView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_TABCONTROL 100

// CPasswordManageView

IMPLEMENT_DYNCREATE(CPasswordManageView, CView)

BEGIN_MESSAGE_MAP(CPasswordManageView, CView)
	// 标准打印命令
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CPasswordManageView 构造/析构

CPasswordManageView::CPasswordManageView() noexcept
{
	// TODO: 在此处添加构造代码

}

CPasswordManageView::~CPasswordManageView()
{
}

BOOL CPasswordManageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPasswordManageView 绘图

void CPasswordManageView::OnDraw(CDC* /*pDC*/)
{
	CPasswordManageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPasswordManageView 打印

BOOL CPasswordManageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPasswordManageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPasswordManageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPasswordManageView 诊断

#ifdef _DEBUG
void CPasswordManageView::AssertValid() const
{
	CView::AssertValid();
}

void CPasswordManageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPasswordManageDoc* CPasswordManageView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPasswordManageDoc)));
	return (CPasswordManageDoc*)m_pDocument;
}
#endif //_DEBUG


// CPasswordManageView 消息处理程序

int CPasswordManageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	m_wndTabControl.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_TABCONTROL);
;
	AddGroup(_T("11111"));
	m_wndTabControl.SetCurSel(0);
	return 0;
}

BOOL CPasswordManageView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle)
{
	CCreateContext contextT;
	contextT.m_pCurrentDoc = GetDocument();
	contextT.m_pNewViewClass = pViewClass;
	contextT.m_pNewDocTemplate = GetDocument()->GetDocTemplate();

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

BOOL CPasswordManageView::AddGroup(LPCTSTR lpszTitle)
{
	BOOL Result = AddView(RUNTIME_CLASS(CPasswordView), lpszTitle);
	return Result;
}

void CPasswordManageView::UpdateDocTitle()
{
	GetDocument()->UpdateFrameCounts();
}