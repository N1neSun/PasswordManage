#include "CSetInfoDlg.h"



IMPLEMENT_DYNAMIC(CSetInfo, CDialogEx)

CSetInfo::CSetInfo(const PasswordColumnInfo& info) : CDialogEx(CSetInfo::IDD)
{
	m_strName = info.Name.c_str();
	m_strUsername = info.Username.c_str();
	m_strPassword = info.Password.c_str();
	m_strUrl = info.Url.c_str();
	m_strNotes = info.Notes.c_str();
	m_strGroup = info.GroupName.c_str();
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
END_MESSAGE_MAP()

BOOL CSetInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void CSetInfo::OnOK()
{
	UpdateData(TRUE);
	m_PasswordInfo.Name = m_strName.GetBuffer();
	m_PasswordInfo.Username = m_strUsername.GetBuffer();
	m_PasswordInfo.Password = m_strPassword.GetBuffer();
	m_PasswordInfo.Url = m_strUrl.GetBuffer();
	m_PasswordInfo.Notes = m_strNotes.GetBuffer();
	m_PasswordInfo.GroupName = m_strGroup.GetBuffer();
	if (SqliteDatabase::GetDBController().IsExist(m_strName.GetBuffer()))
	{
		SqliteDatabase::GetDBController().UpdateControlInfo(m_PasswordInfo);
	}
	else
	{
		SqliteDatabase::GetDBController().InsertPasswordInfo(m_PasswordInfo);
	}
	EndDialog(0);
}