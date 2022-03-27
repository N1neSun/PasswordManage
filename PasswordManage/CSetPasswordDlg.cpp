#include "CSetPasswordDlg.h"
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
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()

BOOL CSetPassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void CSetPassword::OnOK()
{
	UpdateData();

	if (m_strPassword || m_strConfirmPassword)
	{
		return;
	}

	if (m_strPassword != m_strConfirmPassword)
	{
		AfxMessageBox(_T("密码不一致！"));
		return;
	}
	char szUUID[KEY_MAX_LEN] = { 0 };
	CreateUUID(szUUID);
	if (SaveDecryptKey(m_strPassword, szUUID))
	{
		AfxMessageBox(_T("key创建成功！"));
	}
	else
	{
		AfxMessageBox(_T("key创建失败！"));
	}
}