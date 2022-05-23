
// MainFrm.h: CMainFrame ��Ľӿ�
//

#pragma once

#define CMDIFrameWnd CMDIFrameWndEx

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

	// ����
public:

	// ����
public:

	// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar	m_wndMenuBar;
	CMFCStatusBar	m_wndStatusBar;
	CMFCToolBar	m_wndToolBar;

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSyncSettings();
	DECLARE_MESSAGE_MAP()

};


