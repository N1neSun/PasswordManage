#include "CSetInfoDlg.h"


IMPLEMENT_DYNAMIC(CSetInfo, CDialogEx)

CSetInfo::CSetInfo() : CDialogEx(CSetInfo::IDD)
{

}

CSetInfo::~CSetInfo()
{

}

void CSetInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_Username);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_Password);
	DDX_Control(pDX, IDC_EDIT_URL, m_URL);
	DDX_Control(pDX, IDC_EDIT_NOTES, m_Notes);
}


BEGIN_MESSAGE_MAP(CSetInfo, CDialogEx)
END_MESSAGE_MAP()