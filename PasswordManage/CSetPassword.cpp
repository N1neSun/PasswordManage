#include "CSetPassword.h"
#include "util.h"

IMPLEMENT_DYNAMIC(CSetPassword, CDialogEx)

CSetPassword::CSetPassword() : CDialogEx(CSetPassword::IDD)
{

}

CSetPassword::~CSetPassword()
{

}

void CSetPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_SETPASSWORD, m_SetPasswordEdit);
	DDX_Control(pDX, IDC_EDIT_CONFIRMPASSWORD, m_ConfirmPasswordEdit);

	DDX_Text(pDX, IDC_EDIT_SETPASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_CONFIRMPASSWORD, m_strConfirmPassword);
}


BEGIN_MESSAGE_MAP(CSetPassword, CDialogEx)
END_MESSAGE_MAP()

BOOL CSetPassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	return TRUE;
}
