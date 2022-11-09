#include "CExportXshellDlg.h"
#include "util.h"
#include "hash.h"
#include "afxdlgs.h"
#include <shlobj.h>
#include "util.h"

IMPLEMENT_DYNAMIC(CExportXshell, CDialogEx)

CExportXshell::CExportXshell() : CDialogEx(CExportXshell::IDD)
{

}

CExportXshell::~CExportXshell()
{
}

void CExportXshell::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_EXPORTXSHELL, m_strXshellPath);
}

BEGIN_MESSAGE_MAP(CExportXshell, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_VIEWS, &CExportXshell::OnBnClicked)
	ON_BN_CLICKED(IDOK, &CExportXshell::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CExportXshell::OnInitDialog()
{
	CDialogEx::OnInitDialog();


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
	TCHAR buf[200] = { 0 };
	//GetPrivateProfileSection(_T(""), buf, 200, m_strXshellPath);
	GetPrivateProfileString(_T("CONNECTION"), _T("Host"), _T(""), buf, 200, m_strXshellPath);
	std::string key = GetUsernameAndSid();
	std::string password = "";
	std::string base64Decode = base64_decode(password);
	std::string strSha256Key;
	unsigned char szBytesSha256[32] = {0};
	sha256_buffer_byte((unsigned char*)key.c_str(), key.length(), szBytesSha256, 32);
	std::string strData = getNchar(base64Decode, base64Decode.length() - 32);
	unsigned char szDecryptData[64] = { 0 };
	RC4_Decode(szBytesSha256, (unsigned char*)strData.c_str(), szDecryptData);
}
