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


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};