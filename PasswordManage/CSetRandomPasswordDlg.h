#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include <map>
#include <string>

typedef void (*RandomFuc)(std::string&, int);
typedef std::map<int, RandomFuc> FcuntionMap;

class CSetRandomPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CSetRandomPassword)

public:
	CSetRandomPassword();
	virtual ~CSetRandomPassword();

	enum {
		IDD = IDD_RAN_PASSWORD
	};
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_PASSWORDINFO
	};
#endif

private:
	CEdit m_SetPasswordEdit;
	CButton m_SetCharCheckBox;
	CButton m_SetLowerCharCheckBox;
	CButton m_SetUpperCharCheckBox;
	BOOL m_IsNum;
	BOOL m_IsChar;
	BOOL m_IsSymbol;
	int m_PasswordCount;
	BOOL m_IsLower;
	BOOL m_IsUpper;
	CListCtrl m_PasswordList;
	CString m_strPassword;
	FcuntionMap m_FunctionMap;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRandom();
};