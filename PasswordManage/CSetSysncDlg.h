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


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:

};