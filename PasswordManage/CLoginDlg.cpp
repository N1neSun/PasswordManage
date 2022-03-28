#include "CLoginDlg.h"
#include "util.h"
#include "CSetPasswordDlg.h"



IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin() : CDialogEx(CLogin::IDD)
{
	m_strKey.clear();
}

CLogin::~CLogin()
{

}

void CLogin::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_LOGIN, m_strLoginPassword);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)

	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	TCHAR szKeyFile[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szKeyFile, MAX_PATH);
	PathRemoveFileSpec(szKeyFile);
	PathAppend(szKeyFile, KEY_FILE);

	if (!PathFileExists(szKeyFile))
	{
		CSetPassword setPasswordDlg;
		setPasswordDlg.DoModal();
	}

	return TRUE;
}

void CLogin::OnBnClickedOk()
{
	UpdateData();
	// TODO: 在此添加控件通知处理程序代码
	m_strKey = m_strLoginPassword.GetBuffer();
	this->EndDialog(0);
}
