
// PasswordManageView.h: CPasswordManageView 类的接口
//

#pragma once


class CPasswordManageView : public CView
{
protected: // 仅从序列化创建
	CPasswordManageView() noexcept;
	DECLARE_DYNCREATE(CPasswordManageView)

	// 特性
public:
	CPasswordManageDoc* GetDocument() const;
	void UpdateDocTitle();

	// 操作
public:
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle);
	BOOL AddGroup(LPCTSTR lpszTitle);
	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	CTabCtrl m_wndTabControl;
	virtual ~CPasswordManageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // PasswordManageView.cpp 中的调试版本
inline CPasswordManageDoc* CPasswordManageView::GetDocument() const
{
	return reinterpret_cast<CPasswordManageDoc*>(m_pDocument);
}
#endif

