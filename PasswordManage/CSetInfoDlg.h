#pragma once

#include "resource.h"
#include "afxdialogex.h"
#include "SqliteDatabase.h"


class CSetInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSetInfo)

public:
	CSetInfo(PasswordColumnInfo* info);
	virtual ~CSetInfo();

	void SetGroupComboBox();

	void SetVersion(std::string strVerison, unsigned int time);

	std::string m_strKey;

	enum {
		IDD = IDD_PASSWORDINFO
	};
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_PASSWORDINFO
};
#endif

private:
	CEdit m_NameEdit;
	CEdit m_UsernameEdit;
	CEdit m_PasswordEdit;
	CEdit m_URLEdit;
	CEdit m_NotesEdit;
	CComboBox m_GroupComboBox;
	CString m_strPasswordID;
	CString m_strName;
	CString m_strUsername;
	CString m_strPassword;
	CString m_strUrl;
	CString m_strNotes;
	CString m_strGroup;
	PasswordColumnInfo m_PasswordInfo;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonRandPass();
};