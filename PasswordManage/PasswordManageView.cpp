
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

IMPLEMENT_DYNCREATE(CPasswordManageView, CTabView)

BEGIN_MESSAGE_MAP(CPasswordManageView, CTabView)
	// ��׼��ӡ����
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT, CTabView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTabView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

// CPasswordManageView ����/����

CPasswordManageView::CPasswordManageView() noexcept
{
	// TODO: �ڴ˴����ӹ������

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

	// TODO: �ڴ˴�Ϊ�����������ӻ��ƴ���
}

void CPasswordManageView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

// CPasswordManageView ��ӡ

BOOL CPasswordManageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPasswordManageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPasswordManageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӵ�ӡ����е���������
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


// CPasswordManageView ��Ϣ��������

int CPasswordManageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	AddView(RUNTIME_CLASS(CPasswordView), _T("1111"), IDC_TABCONTROL);
	// TODO: Add your specialized creation code here
	//m_wndTabControl.Create("1", "2", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_TABCONTROL);
	//m_wndTabControl.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_TABCONTROL);
;
	//AddGroup(_T("11111"));
	//m_wndTabControl.SetCurSel(0);
	return 0;
}

void CPasswordManageView::UpdateDocTitle()
{
	GetDocument()->UpdateFrameCounts();
}