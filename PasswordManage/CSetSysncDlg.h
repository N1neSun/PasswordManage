#pragma once
#pragma once

#include "resource.h"
#include "afxdialogex.h"


class CSetSysnc : public CDialogEx
{
	DECLARE_DYNAMIC(CSetSysnc)

public:
	CSetSysnc();
	virtual ~CSetSysnc();

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
	CButton m_CheckTestButton;
	CButton m_CheckAutoButton;
	CButton m_ApplyButton;

	CString m_strWebDavUrl;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonTesturl();
	afx_msg void OnBnClickedButtonApply();
};