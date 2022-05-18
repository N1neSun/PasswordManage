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

	DDX_Control(pDX, IDC_EDIT_WEBDAVURL, m_WebDavUrlEdit);
	DDX_Control(pDX, IDC_BUTTON_TESTURL, m_CheckTestButton);
	DDX_Control(pDX, IDC_CHECK_AUTOSYSNC, m_CheckAutoButton);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_ApplyButton);

	DDX_Text(pDX, IDC_EDIT_SETPASSWORD, m_strWebDavUrl);
}


BEGIN_MESSAGE_MAP(CSetSysnc, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()

BOOL CSetSysnc::OnInitDialog()
{

}