#include "SyncPassword.h"
#include "SqliteDatabase.h"
#include "JsonObject.h"
#include "hash.h"
#include "util.h"
#include<atlstr.h>


SyncPassword::SyncPassword()
{
	if (!ReadSysncConfig())
	{
		return;
	}
	m_vecLocalJsonData.clear();
	m_vecRemoteJsonData.clear();
	m_vecSyncJsonData.clear();
}

SyncPassword::SyncPassword(std::map<std::string, std::string> WebDavOptions, int bAutoSync, std::string strKey)
{
	m_WebDavOptions = WebDavOptions;
	m_strSyncKey = WebDavOptions["webdav_password"];
	m_bAutoSync = bAutoSync;
	m_uSyncJsonTime = 0;
	m_vecLocalJsonData.clear();
	m_vecRemoteJsonData.clear();
	m_vecSyncJsonData.clear();
	m_strKey = strKey;
	m_strSyncRemoteKey = m_strSyncKey;
}

SyncPassword::~SyncPassword()
{

}

bool SyncPassword::SqliteToJsonFile(bool bCopyFile)
{
	bool bRet = false;
	do 
	{
		TCHAR szKeyFile[MAX_PATH] = { 0 };
		TCHAR szSyncLocalFilebak[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szKeyFile, MAX_PATH);
		GetModuleFileName(NULL, szSyncLocalFilebak, MAX_PATH);
		PathRemoveFileSpec(szKeyFile);
		PathRemoveFileSpec(szSyncLocalFilebak);
		PathAppend(szKeyFile, SYNCDATAFILE);
		PathAppend(szSyncLocalFilebak, SYNCDATAFILEBAK);
		CJson jsFirmware;
		std::vector<PasswordColumnInfo*> vecPasswordInfo;
		SqliteDatabase::GetDBController().GetPasswordInfoList(vecPasswordInfo);
		if (vecPasswordInfo.empty())
		{
			return true;
		}
		//std::vector<std::string> vecTmpJsonData;
		std::string strSyncDataMd5 = "";
		for each (auto info in vecPasswordInfo)
		{
			CJson jsTmpData;
			jsTmpData.AddValue("PasswordId", info->PasswordId);
			m_vecLocalJsonIndex.push_back(info->PasswordId);
			jsTmpData.AddValue("Name", info->Name); 
			jsTmpData.AddValue("Username", info->Username);
			jsTmpData.AddValue("Password", aes_256_cbc_decode(m_strKey, base64_decode(info->Password)));
			jsTmpData.AddValue("Url", info->Url);
			jsTmpData.AddValue("GroupName", info->GroupName);
			jsTmpData.AddValue("Notes", info->Notes);
			jsTmpData.AddValue("Isdelete", info->Isdelete);
			m_vecLocalJsonData.push_back(jsTmpData.FastWrite());
			//jsFirmware.AddArrayValue("data", jsTmpData.FastWrite());
		}
		jsFirmware.AddArrayValue("data", m_vecLocalJsonData);
		md5_buffer_string((const unsigned char*)jsFirmware.FastWrite().c_str(), jsFirmware.FastWrite().size(), strSyncDataMd5);
		if (strSyncDataMd5.empty())
			return bRet;
		jsFirmware.AddValue(SYNCVERSION, strSyncDataMd5);
		time_t timeSync = time(0);
		unsigned int uTmpTime = (unsigned int)timeSync;
		jsFirmware.AddValue(SYNCTIME, uTmpTime);

		if (!WriteStringToFile(szKeyFile, aes_256_cbc_encode(m_strSyncRemoteKey,jsFirmware.FastWrite())))
			return bRet;
		if(bCopyFile)
			CopyFile(szKeyFile, szSyncLocalFilebak, FALSE);
		SqliteDatabase::GetDBController().SetVersionInfo(strSyncDataMd5);
		SqliteDatabase::GetDBController().SetSyncTimeInfo(uTmpTime);
	} while (FALSE);
	return true;
}

bool SyncPassword::JsonFileToSqlite()
{
	bool bRet = false;
	std::string strJsonVersionIndb;
	unsigned int uTime;
	SqliteDatabase::GetDBController().GetVersionInfo(strJsonVersionIndb);
	SqliteDatabase::GetDBController().GetSyncTimeInfo(uTime);
	do
	{
		TCHAR szSysncFile[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szSysncFile, MAX_PATH);
		PathRemoveFileSpec(szSysncFile);
		PathAppend(szSysncFile, SYNCDATAFILE);
		CJson jsFirmware;
		std::string strEncodeJsonData;
		std::vector<std::string> vecTmpJsonData;
		if (!ReadFileToString(szSysncFile, strEncodeJsonData))
			return bRet;
		std::string strJsonData = aes_256_cbc_decode(m_strSyncRemoteKey, strEncodeJsonData);
		jsFirmware.Parse(strJsonData.c_str());
		if (!jsFirmware.IsCorrectValue())
			return bRet;
		std::string strJsonVersion = jsFirmware.GetStringValue(SYNCVERSION);
		unsigned int uJsonTime = jsFirmware.GetUintValue(SYNCTIME);
		if (strJsonVersionIndb == strJsonVersion)
		{
			return true;
		}
		if (uJsonTime < uTime)
		{
			return true;
		}
		jsFirmware.GetArrayValue("data", vecTmpJsonData);
		for each (auto jsinfo in vecTmpJsonData)
		{
			CJson jsTmpData;
			PasswordColumnInfo info;
			jsTmpData.Parse(jsinfo.c_str());
			if (!jsTmpData.IsCorrectValue())
				return bRet;
			info.PasswordId = jsTmpData.GetStringValue("PasswordId");
			info.Name = jsTmpData.GetStringValue("Name");
			info.Username = jsTmpData.GetStringValue("Username");
			std::string tmppassword = jsTmpData.GetStringValue("Password");
			info.Password = aes_256_cbc_encode(m_strKey, base64_encode(tmppassword.c_str(), tmppassword.length()));
			info.Url = jsTmpData.GetStringValue("Url");
			info.GroupName = jsTmpData.GetStringValue("GroupName");
			if (SqliteDatabase::GetDBController().IsExist(info.PasswordId))
			{
				SqliteDatabase::GetDBController().UpdateControlInfo(info);
			}
			else
			{
				SqliteDatabase::GetDBController().InsertPasswordInfo(info);
			}
		}
		SqliteDatabase::GetDBController().SetVersionInfo(strJsonVersion);
		SqliteDatabase::GetDBController().SetSyncTimeInfo(uJsonTime);
	} while (FALSE);
	return true;
}

int SyncPassword::DownloadRemoteJsonData()
{
	int bRet = -1;
	do 
	{
		TCHAR szTmpDataFile[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szTmpDataFile, MAX_PATH);
		PathRemoveFileSpec(szTmpDataFile);
		PathAppend(szTmpDataFile, SYNCTMPDATAFILE);
		if (m_WebDavOptions.empty())
			return bRet;
		std::unique_ptr<WebDAV::Client> client{ new WebDAV::Client{ m_WebDavOptions } };
		
		CJson jsFirmware;
		char* szTmpDownloadEncodeData;
		unsigned long long buffer_size;
		if (client->download_to(REMOTEFILE, szTmpDownloadEncodeData, buffer_size))
		{
			std::string strDownloadData = aes_256_cbc_decode(m_strSyncRemoteKey, szTmpDownloadEncodeData);
			jsFirmware.Parse(strDownloadData.c_str());
			if (!jsFirmware.IsCorrectValue())
				return bRet;
			m_strSyncJsonVerison = jsFirmware.GetStringValue(SYNCVERSION);
			m_uSyncJsonTime = jsFirmware.GetUintValue(SYNCTIME);
			jsFirmware.GetArrayValue("data", m_vecRemoteJsonData);
			for each (auto jsinfo in m_vecRemoteJsonData)
			{
				CJson jsTmpData;
				PasswordColumnInfo info;
				jsTmpData.Parse(jsinfo.c_str());
				if (!jsTmpData.IsCorrectValue())
					return bRet;
				m_vecRemoteJsonIndex.push_back(jsTmpData.GetStringValue("PasswordId"));
			}
			if (!WriteStringToFile(szTmpDataFile, jsFirmware.FastWrite()))
				return bRet;
		}
		else {
			return 2;
		}
	} while (FALSE);
	return 1;
}

bool SyncPassword::ReadSysncConfig()
{
	bool bRet = false;
	do 
	{
		TCHAR szConfigFile[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szConfigFile, MAX_PATH);
		PathRemoveFileSpec(szConfigFile);
		PathAppend(szConfigFile, SYNCONFIG_FILE);
		CJson jsFirmware;
		std::string strSyncConfigData;
		if (!ReadFileToString(szConfigFile, strSyncConfigData))
			return bRet;
		jsFirmware.Parse(strSyncConfigData.c_str());
		if (!jsFirmware.IsCorrectValue())
			return bRet;
		m_strSyncKey = jsFirmware.GetStringValue(WEBDAVPASSWORD);
		m_WebDavOptions =
		{
		  {"webdav_hostname", jsFirmware.GetStringValue(WEBDAVURL)},
		  {"webdav_username", jsFirmware.GetStringValue(WEBDAVUSER)},
		  {"webdav_password", jsFirmware.GetStringValue(WEBDAVPASSWORD)}
		};
		m_bAutoSync = jsFirmware.GetIntValue(AUTOSYSNC);
		m_strSyncRemoteKey = m_strSyncKey;
	} while (FALSE);
	return true;
}

bool SyncPassword::SyncJsonFile()
{
	bool bRet = false;
	do 
	{
		TCHAR szSyncLocalFile[MAX_PATH] = { 0 };
		TCHAR szSyncRemoteFile[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szSyncLocalFile, MAX_PATH);
		GetModuleFileName(NULL, szSyncRemoteFile, MAX_PATH);
		PathRemoveFileSpec(szSyncLocalFile);
		PathRemoveFileSpec(szSyncRemoteFile);
		PathAppend(szSyncLocalFile, SYNCDATAFILE);
		PathAppend(szSyncRemoteFile, SYNCTMPDATAFILE);
		CJson jsFirmware;
		std::string strSyncLocalData;
		if (!ReadFileToString(szSyncLocalFile, strSyncLocalData))
		{
			CompareSyncFile(m_vecLocalJsonData, m_vecRemoteJsonData);
		}
		else {
			std::string strTmp = aes_256_cbc_decode(m_strSyncRemoteKey, strSyncLocalData).c_str();
			jsFirmware.Parse(strTmp.c_str());
			if (!jsFirmware.IsCorrectValue())
				return bRet;
			std::string strJsonVersion = jsFirmware.GetStringValue(SYNCVERSION);
			unsigned int uJsonTime = jsFirmware.GetUintValue(SYNCTIME);
			if (m_strSyncJsonVerison == strJsonVersion)
			{
				return true;
			}
			if (uJsonTime < m_uSyncJsonTime)
			{
				CompareSyncFile(m_vecLocalJsonData, m_vecRemoteJsonData);
			}
			else
			{
				CompareSyncFile(m_vecRemoteJsonData, m_vecLocalJsonData);
			}
		}

		for each (auto jsinfo in m_vecSyncJsonData)
		{
			CJson jsTmpData;
			PasswordColumnInfo info;
			jsTmpData.Parse(jsinfo.c_str());
			if (!jsTmpData.IsCorrectValue())
				return bRet;
			info.PasswordId = jsTmpData.GetStringValue("PasswordId");
			info.Name = jsTmpData.GetStringValue("Name");
			info.Username = jsTmpData.GetStringValue("Username");
			std:string strTmp = aes_256_cbc_encode(m_strKey, jsTmpData.GetStringValue("Password"));
			info.Password = base64_encode(strTmp.c_str(), strTmp.length());
			info.Url = jsTmpData.GetStringValue("Url");
			info.GroupName = jsTmpData.GetStringValue("GroupName");
			info.Notes = jsTmpData.GetStringValue("Notes");
			info.Isdelete = jsTmpData.GetIntValue("Isdelete");
			if (SqliteDatabase::GetDBController().IsExist(info.PasswordId))
			{
				SqliteDatabase::GetDBController().UpdateControlInfo(info);
			}
			else
			{
				SqliteDatabase::GetDBController().InsertPasswordInfo(info);
			}
		}
		CJson jsTmpFirmware;
		std::string strSyncDataMd5 = "";
		jsTmpFirmware.AddArrayValue("data", m_vecSyncJsonData);
		md5_buffer_string((const unsigned char*)jsTmpFirmware.FastWrite().c_str(), jsTmpFirmware.FastWrite().size(), strSyncDataMd5);
		if (strSyncDataMd5.empty())
			return bRet;
		time_t timeSync = time(0);
		unsigned int uTmpTime = (unsigned int)timeSync;
		SqliteDatabase::GetDBController().SetVersionInfo(strSyncDataMd5);
		SqliteDatabase::GetDBController().SetSyncTimeInfo(uTmpTime);
	} while (FALSE);

	return true;
}

bool SyncPassword::CompareSyncFile(const std::vector<std::string> vecSrc, const std::vector<std::string> vecDes)
{
	std::vector<std::string> vecTmpCompareIndex;
	vecTmpCompareIndex.insert(vecTmpCompareIndex.end(), m_vecLocalJsonIndex.begin(), m_vecLocalJsonIndex.end());
	vecTmpCompareIndex.insert(vecTmpCompareIndex.end(), m_vecRemoteJsonIndex.begin(), m_vecRemoteJsonIndex.end());
	std::vector<std::string> vecCompareIndex = vector_distinct(vectorToset_distinct(vecTmpCompareIndex));
	
	for each (auto jsinfo in vecDes)
	{
		CJson jsTmpData;
		PasswordColumnInfo info;
		jsTmpData.Parse(jsinfo.c_str());
		if (!jsTmpData.IsCorrectValue())
			return false;
		
		auto iter = std::find(vecCompareIndex.begin(), vecCompareIndex.end(), jsTmpData.GetStringValue("PasswordId"));
		if (iter != vecCompareIndex.end())
		{
			vecCompareIndex.erase(remove(vecCompareIndex.begin(), vecCompareIndex.end(), jsTmpData.GetStringValue("PasswordId")), vecCompareIndex.end());
			m_vecSyncJsonData.push_back(jsinfo);
		}
	}
	if (!vecCompareIndex.empty())
	{
		for each (auto jsinfo in vecSrc)
		{
			CJson jsTmpData;
			PasswordColumnInfo info;
			jsTmpData.Parse(jsinfo.c_str());
			if (!jsTmpData.IsCorrectValue())
				return false;

			auto iter = std::find(vecCompareIndex.begin(), vecCompareIndex.end(), jsTmpData.GetStringValue("PasswordId"));
			if (iter != vecCompareIndex.end())
			{
				vecCompareIndex.erase(remove(vecCompareIndex.begin(), vecCompareIndex.end(), jsTmpData.GetStringValue("PasswordId")), vecCompareIndex.end());
				m_vecSyncJsonData.push_back(jsinfo);
			}
			if (vecCompareIndex.empty())
			{
				break;
			}
		}
	}
	return true;
}