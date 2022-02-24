#pragma once
#include "resource.h"
#include <iostream>
#include "afxdialogex.h"

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin();
	virtual ~CLogin();

	std::string getToken() { return m_strKey; }
	std::string setToken(std::string strToken) { m_strKey = strToken; }

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
	std::string m_strKey;

	DECLARE_MESSAGE_MAP()
};