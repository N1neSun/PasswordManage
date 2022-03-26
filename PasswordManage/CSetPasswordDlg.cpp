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

	TCHAR szKeyFile[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szKeyFile, MAX_PATH);
	PathRemoveFileSpec(szKeyFile);
	PathAppend(szKeyFile, KEY_FILE);

	if (!PathFileExists(szKeyFile))
	{
		m_hFile = CreateFile(szKeyFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

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
		AfxMessageBox(_T("���벻һ�£�"));
		return;
	}
	char szUUID[KEY_MAX_LEN] = { 0 };
	CreateUUID(szUUID);
	if (SaveDecryptKey(m_strPassword, szUUID))
	{
		AfxMessageBox(_T("key�����ɹ���"));
	}
	else
	{
		AfxMessageBox(_T("key����ʧ�ܣ�"));
	}
}