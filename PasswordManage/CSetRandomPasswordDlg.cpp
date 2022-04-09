#include "CSetRandomPasswordDlg.h"


IMPLEMENT_DYNAMIC(CSetRandomPassword, CDialogEx)

CSetRandomPassword::CSetRandomPassword() : CDialogEx(CSetRandomPassword::IDD)
{

}

CSetRandomPassword::~CSetRandomPassword()
{

}

void CSetRandomPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_RAN_PASSWORD, m_SetPasswordEdit);
	DDX_Control(pDX, IDC_LIST_PASSWORD, m_PasswordList);

	DDX_Text(pDX, IDC_CHECK_NUM, m_IsNum);
	DDX_Text(pDX, IDC_CHECK_CHAR, m_IsChar);
	DDX_Text(pDX, IDC_CHECK_SYMBOL, m_IsSymbol);
	DDX_Text(pDX, IDC_EDIT_PASS_NUM, m_PasswordCount);
	DDX_Text(pDX, IDC_CHECK_LOWER, m_IsLower);
	DDX_Text(pDX, IDC_CHECK_UPPER, m_IsUpper);
}


BEGIN_MESSAGE_MAP(CSetRandomPassword, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()

BOOL CSetRandomPassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void CSetRandomPassword::OnOK()
{

}