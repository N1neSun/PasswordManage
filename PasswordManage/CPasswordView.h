#pragma once
#include "resource.h"
#include "afxcview.h"

class CPasswordView : public CListView
{
protected:
	CPasswordView();
	virtual ~CPasswordView();
	DECLARE_DYNCREATE(CPasswordView)

	int m_nSortedCol;
	bool m_bAscending;

	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	CListCtrl* m_pListCtrl;
	virtual void OnInitialUpdate();
	void  ShowList();

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};