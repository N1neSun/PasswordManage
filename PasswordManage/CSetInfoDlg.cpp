#include "CPasswordView.h"
#include <afxcontrolbars.h>
#include "PasswordManage.h"
#include "CSetInfoDlg.h"
#include "CSetRandomPasswordDlg.h"

#include "util.h"

extern CTabView* g_pTabView;

IMPLEMENT_DYNAMIC(CSetInfo, CDialogEx)

CSetInfo::CSetInfo(PasswordColumnInfo* info) : CDialogEx(CSetInfo::IDD)
{
	CPasswordManageApp* pApp = (CPasswordManageApp*)AfxGetApp();
	m_strKey = pApp->m_strKey.substr(7);
	if (info == NULL)
		return;
	if (info->id == -1)
		return;
	m_strPasswordID = info->PasswordId.c_str();
	m_strName = info->Name.c_str();
	m_strUsername = info->Username.c_str();
	m_strPassword = aes_256_cbc_decode(m_strKey, info->Password).c_str();
	m_strUrl = info->Url.c_str();
	m_strNotes = info->Notes.c_str();
	m_strGroup = info->GroupName.c_str();

}

CSetInfo::~CSetInfo()
{

}

void CSetInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_NAME, m_NameEdit);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_UsernameEdit);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_PasswordEdit);
	DDX_Control(pDX, IDC_EDIT_URL, m_URLEdit);
	DDX_Control(pDX, IDC_EDIT_NOTES, m_NotesEdit);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_GroupComboBox);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_URL, m_strUrl);
	DDX_Text(pDX, IDC_EDIT_NOTES, m_strNotes);
	DDX_Text(pDX, IDC_COMBO_GROUP, m_strGroup);
}


BEGIN_MESSAGE_MAP(CSetInfo, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
	ON_BN_CLICKED(IDC_BUTTON_RAND_PASS, &CSetInfo::OnBnClickedButtonRandPass)
END_MESSAGE_MAP()

BOOL CSetInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetGroupComboBox();

	CString strTmpGroupName;
	int nIndex = g_pTabView->GetTabControl().GetActiveTab();
	g_pTabView->GetTabControl().GetTabLabel(nIndex, strTmpGroupName);
	int ntab = m_GroupComboBox.FindString(0, strTmpGroupName);
	m_GroupComboBox.SetCurSel(ntab);
	
	return TRUE;
}

BOOL CSetInfo::PreTranslateMessage(MSG* pMsg)
{
	//全选 ctrl+A
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 'A' && GetAsyncKeyState(VK_CONTROL))
	{
		m_NameEdit.SetSel(0, -1);
		m_UsernameEdit.SetSel(0, -1);
		m_PasswordEdit.SetSel(0, -1);
		m_URLEdit.SetSel(0, -1);
		m_NotesEdit.SetSel(0, -1);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CSetInfo::OnOK()
{
	UpdateData(TRUE);
	m_PasswordInfo.PasswordId = m_strPasswordID.GetBuffer();
	m_PasswordInfo.Name = m_strName.GetBuffer();
	m_PasswordInfo.Username = m_strUsername.GetBuffer();
	m_PasswordInfo.Password = aes_256_cbc_encode(m_strKey, m_strPassword.GetBuffer());
	m_PasswordInfo.Url = m_strUrl.GetBuffer();
	m_PasswordInfo.Notes = m_strNotes.GetBuffer();
	m_PasswordInfo.GroupName = m_strGroup.GetBuffer();
	int nTabs = g_pTabView->GetTabControl().GetTabsNum();
	CPasswordView* pView = NULL;
	std::vector<PasswordColumnInfo*> vecPasswordInfolList;
	for (int nIndex = 0; nIndex < nTabs; nIndex++)
	{
		CString strTmpGroupName;
		g_pTabView->GetTabControl().GetTabLabel(nIndex, strTmpGroupName);
		if (strTmpGroupName == m_PasswordInfo.GroupName.c_str())
		{
			pView = DYNAMIC_DOWNCAST(CPasswordView, g_pTabView->GetTabControl().GetTabWnd(nIndex));
		}
	}
	if (pView == NULL)
	{
		g_pTabView->AddView(RUNTIME_CLASS(CPasswordView), m_PasswordInfo.GroupName.c_str());
		pView = DYNAMIC_DOWNCAST(CPasswordView, g_pTabView->GetTabControl().GetTabWnd(nTabs));
		pView->OnInitialUpdate();
	}
	if (SqliteDatabase::GetDBController().IsExist(m_strPasswordID.GetBuffer()))
	{
		SqliteDatabase::GetDBController().UpdateControlInfo(m_PasswordInfo);
	}
	else
	{
		SqliteDatabase::GetDBController().InsertPasswordInfo(m_PasswordInfo);
	}
	SqliteDatabase::GetDBController().GetGroupListInfo(vecPasswordInfolList, m_PasswordInfo.GroupName);
	pView->ShowList(vecPasswordInfolList);
	EndDialog(0);
}

void CSetInfo::SetGroupComboBox()
{
	std::vector<std::string> vecInfo;
	SqliteDatabase::GetDBController().GetGroupInfoList(vecInfo);
	int iIndex = 0;
	if (!vecInfo.size())
	{
		m_GroupComboBox.InsertString(iIndex++, "default");
	}
	else
	{
		for each (auto strGroupName in vecInfo)
		{
			m_GroupComboBox.InsertString(iIndex++, strGroupName.c_str());
		}
	}
}

void CSetInfo::OnBnClickedButtonRandPass()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CSetRandomPassword dlg;
	int nRet = dlg.DoModal();
	if (nRet == 0)
	{
		m_strPassword = dlg.getPassword();
		UpdateData(FALSE);
	}
	else
	{
		return;
	}
}
