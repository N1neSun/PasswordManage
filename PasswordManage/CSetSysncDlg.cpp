#include "CSetSysncDlg.h"
#include "PasswordManage.h"
#include "JsonObject.h"
#include "SqliteDatabase.h"
#include "util.h"

#include <webdav/client.hpp>
#include "SyncPassword.h"



IMPLEMENT_DYNAMIC(CSetSysnc, CDialogEx)

CSetSysnc::CSetSysnc() : CDialogEx(CSetSysnc::IDD)
{
	CPasswordManageApp* pApp = (CPasswordManageApp*)AfxGetApp();
	m_strAppKey = pApp->m_strKey.substr(7);
	if (!SqliteDatabase::GetDBController().SyncInfoIsExist())
	{
		SqliteDatabase::GetDBController().InitSyncInfo();
	}
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
	PathAppend(szConfigFile, SYNCONFIG_FILE);
	if (!PathFileExists(szConfigFile))
		return TRUE;
	std::string strTmpConfig;
	if (!ReadFileToString(szConfigFile, strTmpConfig))
		return FALSE;
	CJson jsFirmware;
	jsFirmware.Parse(strTmpConfig.c_str());
	m_strWebDavUrl = jsFirmware.GetStringValue(WEBDAVURL).c_str();
	m_strWebDavUser = jsFirmware.GetStringValue(WEBDAVUSER).c_str();
	m_strWebDavPassword = aes_256_cbc_decode(m_strAppKey, base64_decode(jsFirmware.GetStringValue(WEBDAVPASSWORD))).c_str();
	if (jsFirmware.GetIntValue(AUTOSYSNC))
	{
		m_bAutoSync = TRUE;
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
	PathAppend(szConfigFile, SYNCONFIG_FILE);
	CJson jsFirmware;
	jsFirmware.AddValue(WEBDAVURL, m_strWebDavUrl);
	jsFirmware.AddValue(WEBDAVUSER, m_strWebDavUser);
	std::string strTmpPassword = aes_256_cbc_encode(m_strAppKey, m_strWebDavPassword.GetBuffer()).c_str();
	jsFirmware.AddValue(WEBDAVPASSWORD, base64_encode(strTmpPassword.c_str(), strTmpPassword.length()).c_str());
	
	if (m_CheckAutoButton.GetCheck())
	{
		m_bAutoSync = TRUE;
		jsFirmware.AddValue(AUTOSYSNC, TRUE);
	}
	else
	{
		m_bAutoSync = FALSE;
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
	UpdateData(TRUE);
	TCHAR szSyncLocalFile[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szSyncLocalFile, MAX_PATH);
	PathRemoveFileSpec(szSyncLocalFile);
	PathAppend(szSyncLocalFile, SYNCDATAFILE);
	if (!(m_strWebDavUrl && m_strWebDavUser && m_strWebDavPassword))
	{
		AfxMessageBox("未进行配置，请配置同步！");
	}
	if (m_WebDavOptions.empty())
	{
		m_WebDavOptions =
		{
		  {"webdav_hostname", m_strWebDavUrl.GetBuffer()},
		  {"webdav_username", m_strWebDavUser.GetBuffer()},
		  {"webdav_password", m_strWebDavPassword.GetBuffer()}
		};
	}
	if (m_CheckAutoButton.GetCheck())
	{
		m_bAutoSync = TRUE;
	}
	else
	{
		m_bAutoSync = FALSE;
	}
	SyncPassword sync(m_WebDavOptions, m_bAutoSync);
	do 
	{
		if (!sync.SqliteToJsonFile())
		{
			AfxMessageBox("本地文件同步出错！");
			return;
		}
		int bRet = sync.DownloadRemoteJsonData();
		if (bRet == -1)
		{
			AfxMessageBox("远端文件同步出错！");
			return;
		}
		if (bRet == 2)
		{
			std::unique_ptr<WebDAV::Client> client{ new WebDAV::Client{ m_WebDavOptions } };
			if (!client->upload(REMOTEFILE, szSyncLocalFile))
			{
				AfxMessageBox("向远端文件同步出错！");
				return;
			}
			AfxMessageBox("同步成功！");
			return;
		}
		if (!sync.SyncJsonFile())
		{
			AfxMessageBox("对比文件同步出错！");
			return;
		}
		
	} while (FALSE);
	AfxMessageBox("同步成功！");
}
