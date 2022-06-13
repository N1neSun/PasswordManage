#pragma once

#include "resource.h"
#include "afxdialogex.h"
#include <string>
#include<map>



class CSetSysnc : public CDialogEx
{
	DECLARE_DYNAMIC(CSetSysnc)

public:
	CSetSysnc();
	virtual ~CSetSysnc();

	std::string m_strAppKey;

	std::map<std::string, std::string> m_WebDavOptions;
	BOOL m_bAutoSync;

	enum {
		IDD = IDD_DIALOG_SYSNC
	};
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_DIALOG_SYSNC
	};
#endif

private:
	CEdit m_WebDavUrlEdit;
	CEdit m_WebDavUserEdit;
	CEdit m_WebDavPasswordEdit;
	CButton m_CheckTestButton;
	CButton m_CheckAutoButton;
	CButton m_SysncButton;
	CButton m_ApplyButton;

	CString m_strWebDavUrl;
	CString m_strWebDavUser;
	CString m_strWebDavPassword;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonTesturl();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonSysnc();
};