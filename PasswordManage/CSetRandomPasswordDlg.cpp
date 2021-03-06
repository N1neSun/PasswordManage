#include "CSetRandomPasswordDlg.h"
#include "util.h"
#include <vector>


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
	DDX_Control(pDX, IDC_CHECK_CHAR, m_SetCharCheckBox);
	DDX_Control(pDX, IDC_CHECK_LOWER, m_SetLowerCharCheckBox);
	DDX_Control(pDX, IDC_CHECK_UPPER, m_SetUpperCharCheckBox);

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
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PASSWORD, OnSelchanged)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CSetRandomPassword::OnBnClickedButtonRandom)
	ON_BN_CLICKED(IDC_CHECK_CHAR, &CSetRandomPassword::OnBnClickedCheckChar)
END_MESSAGE_MAP()

BOOL CSetRandomPassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_PasswordList.SetExtendedStyle(LVS_EX_FULLROWSELECT /*| LVS_EX_FLATSB*/ |
		LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);
	m_PasswordList.InsertColumn(0, _T("????"), LVCFMT_LEFT, 200);
	m_SetLowerCharCheckBox.EnableWindow(FALSE);
	m_SetUpperCharCheckBox.EnableWindow(FALSE);
	return TRUE;
}

BOOL CSetRandomPassword::PreTranslateMessage(MSG* pMsg)
{
	//ȫѡ ctrl+A
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 'A' && GetAsyncKeyState(VK_CONTROL))
	{
		m_SetPasswordEdit.SetSel(0, -1);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CSetRandomPassword::OnOK()
{
	this->EndDialog(0);
}

void CSetRandomPassword::OnBnClickedButtonRandom()
{
	// TODO: ?ڴ????ӿؼ?֪ͨ????????????
	UpdateData(TRUE);
	m_PasswordList.DeleteAllItems();
	std::vector<std::string> vecPasswordList;
	std::string strMark;
	int nTypeCount = 0;
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
			nTypeCount++;
			m_FunctionMap[i](strTmp, m_PasswordCount);
			strPassword += strTmp;
		}
	}
	if (strPassword.empty())
		return;
	for (int nIndex = 0; nIndex < 10; nIndex++)
	{
		//Sleep(100);
		std::string strtemp = GetRandomPassword(strPassword, m_PasswordCount, nTypeCount);
		m_PasswordList.InsertItem(nIndex, "", 0);
		m_PasswordList.SetItemText(nIndex, 0, strtemp.c_str());
	}
	m_strPassword = m_PasswordList.GetItemText(0, 0);
	UpdateData(FALSE);
}


void CSetRandomPassword::OnBnClickedCheckChar()
{
	// TODO: ?ڴ????ӿؼ?֪ͨ????????????
	if (m_SetCharCheckBox.GetCheck())
	{
		m_SetLowerCharCheckBox.EnableWindow(TRUE);
		m_SetUpperCharCheckBox.EnableWindow(TRUE);
		m_SetLowerCharCheckBox.SetCheck(TRUE);
	}
	else
	{
		m_SetLowerCharCheckBox.EnableWindow(FALSE);
		m_SetUpperCharCheckBox.EnableWindow(FALSE);
		m_SetLowerCharCheckBox.SetCheck(FALSE);
	}
	
}

void CSetRandomPassword::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	POSITION pos = m_PasswordList.GetFirstSelectedItemPosition();
	int nIndex = m_PasswordList.GetNextSelectedItem(pos);
	m_strPassword = m_PasswordList.GetItemText(nIndex, 0);
	UpdateData(FALSE);
}
