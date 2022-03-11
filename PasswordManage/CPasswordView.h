#pragma once
#include "resource.h"
#include "afxcview.h"
#include "CSetInfoDlg.h"
#include "SqliteDatabase.h"


class CPasswordView : public CListView
{
protected:
	CPasswordView();
	virtual ~CPasswordView();
	DECLARE_DYNCREATE(CPasswordView)

	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	CListCtrl* m_pListCtrl;

	//CHeaderCtrl m_header;
	virtual void OnInitialUpdate();
	void LoadPasswordInfo();
	void  ShowList(std::vector<PasswordColumnInfo*> vectPasswordInfoList);
	void InsertListInfo(PasswordColumnInfo* info);

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};