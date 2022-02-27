
// PasswordManageView.cpp: CPasswordManageView ���ʵ��
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CPasswordManageView ����/����

CPasswordManageView::CPasswordManageView() noexcept
{
	// TODO: �ڴ˴���ӹ������

}

CPasswordManageView::~CPasswordManageView()
{
}

BOOL CPasswordManageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPasswordManageView ��ͼ

void CPasswordManageView::OnDraw(CDC* /*pDC*/)
{
	CPasswordManageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPasswordManageView ��ӡ

BOOL CPasswordManageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPasswordManageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPasswordManageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CPasswordManageView ���

#ifdef _DEBUG
void CPasswordManageView::AssertValid() const
{
	CView::AssertValid();
}

void CPasswordManageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPasswordManageDoc* CPasswordManageView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPasswordManageDoc)));
	return (CPasswordManageDoc*)m_pDocument;
}
#endif //_DEBUG


// CPasswordManageView ��Ϣ�������

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