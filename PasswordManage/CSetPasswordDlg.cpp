#include "CSetPasswordDlg.h"
#include "util.h"
#include "hash.h"


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

	TCHAR szKeyFile[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szKeyFile, MAX_PATH);
	PathRemoveFileSpec(szKeyFile);
	PathAppend(szKeyFile, KEY_FILE);

	if (!PathFileExists(szKeyFile))
	{
		HANDLE handle = CreateFile(szKeyFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		CloseHandle(handle);
	}


	return TRUE;
}

void CSetPassword::OnOK()
{
	UpdateData();

	if (!m_strPassword || !m_strConfirmPassword)
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
	std::string strMD5Password = "";
	std::string strSignUUID = SIGN;
	md5_buffer_string((const unsigned char*)m_strPassword.GetBuffer(), m_strPassword.GetLength(), strMD5Password);
	if (SaveDecryptKey(strMD5Password.c_str(), strSignUUID + szUUID))
	{
		AfxMessageBox(_T("key创建成功！"));
		this->EndDialog(0);
	}
	else
	{
		AfxMessageBox(_T("key创建失败！"));
		return;
	}
}