#include "CSetSysncDlg.h"

#include "JsonObject.h"
#include "util.h"

IMPLEMENT_DYNAMIC(CSetSysnc, CDialogEx)

CSetSysnc::CSetSysnc() : CDialogEx(CSetSysnc::IDD)
{

}

CSetSysnc::~CSetSysnc()
{

}

void CSetSysnc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_WEBDAVURL, m_WebDavUrlEdit);
	DDX_Control(pDX, IDC_BUTTON_TESTURL, m_CheckTestButton);
	DDX_Control(pDX, IDC_CHECK_AUTOSYSNC, m_CheckAutoButton);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_ApplyButton);

	DDX_Text(pDX, IDC_EDIT_WEBDAVURL, m_strWebDavUrl);
}


BEGIN_MESSAGE_MAP(CSetSysnc, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
	ON_BN_CLICKED(IDC_BUTTON_TESTURL, &CSetSysnc::OnBnClickedButtonTesturl)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSetSysnc::OnBnClickedButtonApply)
END_MESSAGE_MAP()

BOOL CSetSysnc::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(TRUE);
	TCHAR szConfigFile[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szConfigFile, MAX_PATH);
	PathRemoveFileSpec(szConfigFile);
	PathAppend(szConfigFile, SYNNCONFIG_FILE);
	if (!PathFileExists(szConfigFile))
		return TRUE;
	std::string strTmpConfig;
	if (!ReadFileToString(szConfigFile, strTmpConfig))
		return FALSE;
	CJson jsFirmware;
	jsFirmware.Parse(strTmpConfig.c_str());
	m_strWebDavUrl = jsFirmware.GetStringValue(WEBDAVURL).c_str();
	if (jsFirmware.GetIntValue(AUTOSYSNC))
	{
		m_CheckAutoButton.SetCheck(TRUE);
	}
	UpdateData(FALSE);
}

void CSetSysnc::OnBnClickedButtonTesturl()
{
	
}


void CSetSysnc::OnBnClickedButtonApply()
{
	UpdateData(TRUE);
	TCHAR szConfigFile[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szConfigFile, MAX_PATH);
	PathRemoveFileSpec(szConfigFile);
	PathAppend(szConfigFile, SYNNCONFIG_FILE);
	CJson jsFirmware;
	jsFirmware.AddValue(WEBDAVURL, m_strWebDavUrl);
	if (m_CheckAutoButton.GetCheck())
	{
		jsFirmware.AddValue(AUTOSYSNC, TRUE);
	}
	else
	{
		jsFirmware.AddValue(AUTOSYSNC, FALSE);
	}
	if (!WriteStringToFile(szConfigFile, jsFirmware.FastWrite()))
	{
		AfxMessageBox(_T("保存失败!"));
		return;
	}
	AfxMessageBox(_T("保存成功!"));
	this->EndDialog(0);
}
