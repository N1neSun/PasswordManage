#pragma once
#include "resource.h"
#include "afxdialogex.h"

class CSetInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSetInfo)

public:
	CSetInfo();
	virtual ~CSetInfo();

	enum {
		IDD = IDD_PASSWORDINFO
	};
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_PASSWORDINFO
};
#endif

private:
	CEdit m_Name;
	CEdit m_Username;
	CEdit m_Password;
	CEdit m_URL;
	CEdit m_Notes;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};