
// PasswordManageView.h: CPasswordManageView 类的接口
//

#pragma once


class CPasswordManageView : public CTabView
{
protected: // 仅从序列化创建
	CPasswordManageView() noexcept;
	DECLARE_DYNCREATE(CPasswordManageView)

	// 特性
public:
	CPasswordManageDoc* GetDocument() const;
	// 操作
public:

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CPasswordManageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // PasswordManageView.cpp 中的调试版本
inline CPasswordManageDoc* CPasswordManageView::GetDocument() const
{
	return reinterpret_cast<CPasswordManageDoc*>(m_pDocument);
}
#endif

