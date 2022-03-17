#include "CLoginDlg.h"



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

	return TRUE;
}

void CLogin::OnBnClickedOk()
{
	UpdateData();
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	this->EndDialog(0);
}
