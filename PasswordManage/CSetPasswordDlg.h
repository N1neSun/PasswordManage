#pragma once
#include "resource.h"
#include "afxdialogex.h"

class CSetPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CSetPassword)

public:
	CSetPassword();
	virtual ~CSetPassword();

	enum {
		IDD = IDD_DIALOG_SETPASSWORD
	};
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_PASSWORDINFO
	};
#endif

private:
	CEdit m_SetPasswordEdit;
	CEdit m_ConfirmPasswordEdit;
	CString m_strPassword;
	CString m_strConfirmPassword;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()
};