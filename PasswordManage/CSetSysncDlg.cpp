#include "CSetSysncDlg.h"


IMPLEMENT_DYNAMIC(CSetSysnc, CDialogEx)

CSetSysnc::CSetSysnc() : CDialogEx(CSetSysnc::IDD)
{

}

CSetSysnc::~CSetSysnc()
{

}

void CSetSysnc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CSetSysnc, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()

BOOL CSetSysnc::OnInitDialog()
{

}