#include "CSetRandomPasswordDlg.h"
#include "util.h"


IMPLEMENT_DYNAMIC(CSetRandomPassword, CDialogEx)

CSetRandomPassword::CSetRandomPassword() : CDialogEx(CSetRandomPassword::IDD)
{
	m_FunctionMap[0] = GetRandomNum;
	m_FunctionMap[1] = GetRandomLowerChar;
	m_FunctionMap[2] = GetRandomUpperChar;
	m_FunctionMap[3] = GetRandomSymbol;
}

CSetRandomPassword::~CSetRandomPassword()
{

}

void CSetRandomPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_RAN_PASSWORD, m_SetPasswordEdit);
	DDX_Control(pDX, IDC_LIST_PASSWORD, m_PasswordList);

	DDX_Text(pDX, IDC_EDIT_RAN_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_CHECK_NUM, m_IsNum);
	DDX_Check(pDX, IDC_CHECK_CHAR, m_IsChar);
	DDX_Check(pDX, IDC_CHECK_SYMBOL, m_IsSymbol);
	DDX_Text(pDX, IDC_EDIT_PASS_NUM, m_PasswordCount);
	DDX_Check(pDX, IDC_CHECK_LOWER, m_IsLower);
	DDX_Check(pDX, IDC_CHECK_UPPER, m_IsUpper);
}


BEGIN_MESSAGE_MAP(CSetRandomPassword, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CSetRandomPassword::OnBnClickedButtonRandom)
END_MESSAGE_MAP()

BOOL CSetRandomPassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void CSetRandomPassword::OnOK()
{

}

void CSetRandomPassword::OnBnClickedButtonRandom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	std::string strMark;
	int nType = 0;
	int nPassword;
	int nIndex = 0;
	m_IsNum ? strMark += "1" : strMark += "0";
	m_IsLower ? strMark += "1" : strMark += "0";
	m_IsUpper ? strMark += "1" : strMark += "0";
	m_IsSymbol ? strMark += "1" : strMark += "0";

	std::string strTmp;
	std::string strPassword;
	for (int i = 0; i < strMark.length(); i++)
	{
		if (strMark[i] == '1')
		{
			m_FunctionMap[i](strTmp, m_PasswordCount);
			strPassword += strTmp;
		}
	}
}
