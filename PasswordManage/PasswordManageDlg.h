
// PasswordManageDlg.h: 头文件
//

#pragma once


// CPasswordManageDlg 对话框
class CPasswordManageDlg : public CDialogEx
{
// 构造
public:
	CPasswordManageDlg(CWnd* pParent = nullptr);	// 标准构造函数
	CListCtrl m_PasswordList;
	//CTabCtrl	m_PasswordTab;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PASSWORDMANAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	//CListCtrl m_PasswordList;
};
