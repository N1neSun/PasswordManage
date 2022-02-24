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
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)

END_MESSAGE_MAP()

BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}