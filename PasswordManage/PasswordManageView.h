
// PasswordManageView.h: CPasswordManageView ��Ľӿ�
//

#pragma once


class CPasswordManageView : public CView
{
protected: // �������л�����
	CPasswordManageView() noexcept;
	DECLARE_DYNCREATE(CPasswordManageView)

	// ����
public:
	CPasswordManageDoc* GetDocument() const;
	void UpdateDocTitle();

	// ����
public:
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle);
	BOOL AddGroup(LPCTSTR lpszTitle);
	// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// ʵ��
public:
	CTabCtrl m_wndTabControl;
	virtual ~CPasswordManageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // PasswordManageView.cpp �еĵ��԰汾
inline CPasswordManageDoc* CPasswordManageView::GetDocument() const
{
	return reinterpret_cast<CPasswordManageDoc*>(m_pDocument);
}
#endif

