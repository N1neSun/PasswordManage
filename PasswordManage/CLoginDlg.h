#pragma once
#include "resource.h"
#include "afxdialogex.h"

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin();
	virtual ~CLogin();


	enum {
		IDD = IDD_LOGIN
	};
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_LOGIN
	};
#endif

private:



protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()
};