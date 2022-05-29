#include "CSetSysncDlg.h"
#include "PasswordManage.h"
#include "JsonObject.h"
#include "util.h"

#include <webdav/client.hpp>



IMPLEMENT_DYNAMIC(CSetSysnc, CDialogEx)

CSetSysnc::CSetSysnc() : CDialogEx(CSetSysnc::IDD)
{
	CPasswordManageApp* pApp = (CPasswordManageApp*)AfxGetApp();
	m_strAppKey = pApp->m_strKey.substr(7);
}

CSetSysnc::~CSetSysnc()
{

}

void CSetSysnc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_WEBDAVURL, m_WebDavUrlEdit);
	DDX_Control(pDX, IDC_EDIT_WEBDAVUSER, m_WebDavUserEdit);
	DDX_Control(pDX, IDC_EDIT_WEBDAVPASSWORD, m_WebDavPasswordEdit);
	DDX_Control(pDX, IDC_BUTTON_TESTURL, m_CheckTestButton);
	DDX_Control(pDX, IDC_CHECK_AUTOSYSNC, m_CheckAutoButton);
	DDX_Control(pDX, IDC_BUTTON_SYSNC, m_SysncButton);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_ApplyButton);

	DDX_Text(pDX, IDC_EDIT_WEBDAVURL, m_strWebDavUrl);
	DDX_Text(pDX, IDC_EDIT_WEBDAVUSER, m_strWebDavUser);
	DDX_Text(pDX, IDC_EDIT_WEBDAVPASSWORD, m_strWebDavPassword);
}


BEGIN_MESSAGE_MAP(CSetSysnc, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
	ON_BN_CLICKED(IDC_BUTTON_TESTURL, &CSetSysnc::OnBnClickedButtonTesturl)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSetSysnc::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_SYSNC, &CSetSysnc::OnBnClickedButtonSysnc)
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
	m_strWebDavUser = jsFirmware.GetStringValue("User").c_str();
	m_strWebDavPassword = aes_256_cbc_decode(m_strAppKey, base64_decode(jsFirmware.GetStringValue("Password"))).c_str();
	if (jsFirmware.GetIntValue(AUTOSYSNC))
	{
		m_CheckAutoButton.SetCheck(TRUE);
	}
	UpdateData(FALSE);
}

void CSetSysnc::OnBnClickedButtonTesturl()
{
	UpdateData(TRUE);
	m_WebDavOptions =
	{
	  {"webdav_hostname", m_strWebDavUrl.GetBuffer()},
	  {"webdav_username", m_strWebDavUser.GetBuffer()},
	  {"webdav_password", m_strWebDavPassword.GetBuffer()}
	};
	std::unique_ptr<WebDAV::Client> client{ new WebDAV::Client{ m_WebDavOptions } };

	bool check_connection = client->check();
	if (check_connection)
	{
		AfxMessageBox("配置测试成功!");
	}
	else {
		AfxMessageBox("配置测试失败!");
	}
	UpdateData(FALSE);
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
	jsFirmware.AddValue("User", m_strWebDavUser);
	std::string strTmpPassword = aes_256_cbc_encode(m_strAppKey, m_strWebDavPassword.GetBuffer()).c_str();
	jsFirmware.AddValue("Password", base64_encode(strTmpPassword.c_str(), strTmpPassword.length()).c_str());
	
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
	//this->EndDialog(0);
}


void CSetSysnc::OnBnClickedButtonSysnc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_WebDavOptions.empty())
	{
		m_WebDavOptions =
		{
		  {"webdav_hostname", m_strWebDavUrl.GetBuffer()},
		  {"webdav_username", m_strWebDavUser.GetBuffer()},
		  {"webdav_password", m_strWebDavPassword.GetBuffer()}
		};
	}
	TCHAR szSyncDataFile[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szSyncDataFile, MAX_PATH);
	PathRemoveFileSpec(szSyncDataFile);
	PathAppend(szSyncDataFile, SYNCDATAFILE);
	std::unique_ptr<WebDAV::Client> client{ new WebDAV::Client{ m_WebDavOptions } };

	if (client->upload(REMOTEFILE, szSyncDataFile))
	{
		AfxMessageBox("同步成功!");
	}
	else
	{
		AfxMessageBox("同步失败!");
	}
}
