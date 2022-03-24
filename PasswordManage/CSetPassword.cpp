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


}


BEGIN_MESSAGE_MAP(CSetPassword, CDialogEx)
END_MESSAGE_MAP()

BOOL CSetPassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	return TRUE;
}
