#pragma once
#include "resource.h"
#include <iostream>
#include "afxdialogex.h"
#include "SqliteDatabase.h"

class CExportXshell : public CDialogEx
{
	DECLARE_DYNAMIC(CExportXshell)

public:
	CExportXshell();
	virtual ~CExportXshell();
	
	std::string m_strKey;


	enum {
		IDD = IDD_EXPORTXSHELL
	};
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_EXPORTXSHELL
	};
#endif

private:



protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	CString m_strXshellPath;
	CString m_strXshellVersion;
	CString m_strExportGroup;

	CComboBox m_combVersionComboBox;
	CComboBox m_combExportGroupComboBox;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
	afx_msg void OnBnClickedOk();
};