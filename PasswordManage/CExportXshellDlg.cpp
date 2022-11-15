#include "CExportXshellDlg.h"
#include "util.h"
#include "hash.h"
#include "afxdlgs.h"
#include <shlobj.h>
#include "util.h"
#include "CPasswordView.h"
#include <afxcontrolbars.h>
#include "PasswordManage.h"

extern CTabView* g_pTabView;

IMPLEMENT_DYNAMIC(CExportXshell, CDialogEx)

CExportXshell::CExportXshell() : CDialogEx(CExportXshell::IDD)
{
	CPasswordManageApp* pApp = (CPasswordManageApp*)AfxGetApp();
	m_strKey = pApp->m_strKey.substr(7);
}

CExportXshell::~CExportXshell()
{
}

void CExportXshell::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_EXPORTXSHELL, m_strXshellPath);
	DDX_Text(pDX, IDC_COMBO_EXPORTXSHELL, m_strXshellVersion); 
	DDX_Text(pDX, IDC_COMBO_EXPORTGROUP, m_strExportGroup);

	DDX_Control(pDX, IDC_COMBO_EXPORTXSHELL, m_combVersionComboBox);
	DDX_Control(pDX, IDC_COMBO_EXPORTGROUP, m_combExportGroupComboBox);
}

BEGIN_MESSAGE_MAP(CExportXshell, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_VIEWS, &CExportXshell::OnBnClicked)
	ON_BN_CLICKED(IDOK, &CExportXshell::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CExportXshell::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_combVersionComboBox.SetCurSel(0);

	std::vector<std::string> vecInfo;
	SqliteDatabase::GetDBController().GetGroupInfoList(vecInfo);
	int iIndex = 0;
	if (!vecInfo.size())
	{
		m_combExportGroupComboBox.InsertString(iIndex++, "default");
	}
	else
	{
		for each (auto strGroupName in vecInfo)
		{
			m_combExportGroupComboBox.InsertString(iIndex++, strGroupName.c_str());
		}
	}

	CString strTmpGroupName;
	int nIndex = g_pTabView->GetTabControl().GetActiveTab();
	g_pTabView->GetTabControl().GetTabLabel(nIndex, strTmpGroupName);
	int ntab = m_combExportGroupComboBox.FindString(0, strTmpGroupName);
	m_combExportGroupComboBox.SetCurSel(ntab);

	return TRUE;
}

void CExportXshell::OnBnClicked()
{
	UpdateData();
	TCHAR szFolderPath[MAX_PATH] = { 0 };
	BROWSEINFO sInfo;
	ZeroMemory(&sInfo, sizeof(BROWSEINFO));

	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择一个文件夹：");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框
	LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名
		if (SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			m_strXshellPath = szFolderPath;
		}
	}
	if (lpidlBrowse != NULL)
	{
		CoTaskMemFree(lpidlBrowse);
	}
	UpdateData(FALSE);

}

void CExportXshell::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_strXshellPath.GetLength() == 0){
		AfxMessageBox(_T("请先选择导入xhell配置文件的目录!"));
		return;
	}
	//C:\Users\N1neSun\Documents\NetSarang Computer\7\Xshell\Sessions\1
	std::vector<std::string> vFileFullPath;
	readFileNameInDir(m_strXshellPath.GetBuffer(0),vFileFullPath);
	int nTabs = g_pTabView->GetTabControl().GetTabsNum();
	CPasswordView* pView = NULL;
	std::vector<PasswordColumnInfo*> vecPasswordInfolList;
	for (int nIndex = 0; nIndex < nTabs; nIndex++)
	{
		CString strTmpGroupName;
		g_pTabView->GetTabControl().GetTabLabel(nIndex, strTmpGroupName);
		if (strTmpGroupName == m_strExportGroup)
		{
			pView = DYNAMIC_DOWNCAST(CPasswordView, g_pTabView->GetTabControl().GetTabWnd(nIndex));
		}
	}
	if (pView == NULL)
	{
		g_pTabView->AddView(RUNTIME_CLASS(CPasswordView), m_strExportGroup);
		pView = DYNAMIC_DOWNCAST(CPasswordView, g_pTabView->GetTabControl().GetTabWnd(nTabs));
		pView->OnInitialUpdate();
	}
	for each(auto strFilename in vFileFullPath){
		std::string strTmpSlash = "\\";
		std::string strTempPath = m_strXshellPath.GetBuffer(0) + strTmpSlash + strFilename;
		TCHAR szHost[200] = { 0 };
		TCHAR szPort[200] = { 0 };
		TCHAR szPassword[200] = { 0 };
		TCHAR szUserName[200] = { 0 };
		TCHAR szUrl[200] = { 0 };
		bool bReverse = false;
		//GetPrivateProfileSection(_T(""), buf, 200, m_strXshellPath);
		GetPrivateProfileString(CONNECTION, HOST, _T(""), szHost, 200, strTempPath.c_str());
		GetPrivateProfileString(CONNECTION, PORT, _T(""), szPort, 200, strTempPath.c_str());
		GetPrivateProfileString(AUTHENTICATION, USERNAME, _T(""), szUserName, 200, strTempPath.c_str());
		GetPrivateProfileString(AUTHENTICATION, PASSWORD, _T(""), szPassword, 200, strTempPath.c_str());
		if (m_strXshellVersion == XSHELL7)
		{
			bReverse = true;
		}
		std::string key = GetUsernameAndSid(bReverse);
		std::string password = szPassword;
		std::string base64Decode = base64_decode(password);
		unsigned char szBytesSha256[32] = { 0 };
		sha256_buffer_byte((unsigned char*)key.c_str(), key.length(), szBytesSha256, 32);
		std::string strData = getNchar(base64Decode, base64Decode.length() - 32);
		unsigned char szDecryptData[64] = { 0 };
		RC4_Decode(szBytesSha256, (unsigned char*)strData.c_str(), szDecryptData);

		PasswordColumnInfo passwordInfo;
		passwordInfo.Name = szHost;
		passwordInfo.Username = szUserName;
		std::string strTmp = aes_256_cbc_encode(m_strKey, (char*)szDecryptData);
		passwordInfo.Password = base64_encode(strTmp.c_str(), strTmp.length()).c_str();
		sprintf(szUrl, "%s:%s", szHost, szPort);
		passwordInfo.Url = szUrl;
		passwordInfo.GroupName = m_strExportGroup;
		if (SqliteDatabase::GetDBController().IsExistByName(szHost))
		{
			SqliteDatabase::GetDBController().UpdateControlInfoForXshell(passwordInfo);
		}
		else
		{
			SqliteDatabase::GetDBController().InsertPasswordInfo(passwordInfo);
		}
	}
	SqliteDatabase::GetDBController().GetGroupListInfo(vecPasswordInfolList, m_strExportGroup.GetBuffer(0));
	pView->ShowList(vecPasswordInfolList);

}
