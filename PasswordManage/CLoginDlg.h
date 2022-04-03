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

	std::string getKey() { return m_strKey; }
	std::string setKey(std::string strKey) { m_strKey = strKey; }

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
	CString m_strLoginPassword;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};