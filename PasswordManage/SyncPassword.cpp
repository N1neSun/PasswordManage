#include "SyncPassword.h"
#include "SqliteDatabase.h"
#include "JsonObject.h"
#include "hash.h"
#include "util.h"
#include<atlstr.h>
#include <webdav/client.hpp>

SyncPassword::SyncPassword()
{
	if (!ReadSysncConfig())
	{
		return;
	}
}

SyncPassword::~SyncPassword()
{

}

bool SyncPassword::SqliteToJsonFile()
{
	bool bRet = false;
	do 
	{
		TCHAR szKeyFile[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szKeyFile, MAX_PATH);
		PathRemoveFileSpec(szKeyFile);
		PathAppend(szKeyFile, SYNCDATAFILE);
		CJson jsFirmware;
		std::vector<PasswordColumnInfo*> vecPasswordInfo;
		SqliteDatabase::GetDBController().GetPasswordInfoList(vecPasswordInfo);
		if (vecPasswordInfo.empty())
		{
			return bRet;
		}
		std::vector<std::string> vecTmpJsonData;
		std::string strSyncDataMd5 = "";
		for each (auto info in vecPasswordInfo)
		{
			CJson jsTmpData;
			jsTmpData.AddValue("PasswordId", info->PasswordId);
			jsTmpData.AddValue("Name", info->Name);
			jsTmpData.AddValue("Username", info->Username);
			jsTmpData.AddValue("Password", info->Password);
			jsTmpData.AddValue("Url", info->Url);
			jsTmpData.AddValue("GroupName", info->GroupName);
			vecTmpJsonData.push_back(jsTmpData.FastWrite());
			//jsFirmware.AddArrayValue("data", jsTmpData.FastWrite());
		}
		jsFirmware.AddArrayValue("data", vecTmpJsonData);
		md5_buffer_string((const unsigned char*)jsFirmware.FastWrite().c_str(), jsFirmware.FastWrite().size(), strSyncDataMd5);
		if (strSyncDataMd5.empty())
			return bRet;
		jsFirmware.AddValue(SYNCVERSION, strSyncDataMd5);
		time_t timeSync = time(0);
		unsigned int uTmpTime = (unsigned int)timeSync;
		jsFirmware.AddValue(SYNCTIME, uTmpTime);

		if (!WriteStringToFile(szKeyFile, jsFirmware.FastWrite()))
			return bRet;
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
		std::string strJsonData;
		std::vector<std::string> vecTmpJsonData;
		if (!ReadFileToString(szSysncFile, strJsonData))
			return bRet;
		jsFirmware.Parse(strJsonData.c_str());
		std::string strJsonVersion = jsFirmware.GetStringValue(SYNCVERSION);
		unsigned int uJsonTime = jsFirmware.GetUintValue(SYNCTIME);
		if (strJsonVersionIndb == strJsonVersion)
		{
			return true;
		}
		if (uJsonTime < uTime)
		{
			return false;
		}
		jsFirmware.GetArrayValue("data", vecTmpJsonData);
		for each (auto jsinfo in vecTmpJsonData)
		{
			CJson jsTmpData;
			PasswordColumnInfo info;
			jsTmpData.Parse(jsinfo.c_str());
			info.PasswordId = jsTmpData.GetStringValue("PasswordId");
			info.Name = jsTmpData.GetStringValue("Name");
			info.Username = jsTmpData.GetStringValue("Username");
			info.Password = jsTmpData.GetStringValue("Password");
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
	} while (FALSE);
	return true;
}

bool SyncPassword::DownloadRemoteJsonData()
{
	bool bRet = false;
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
		char* szTmpDownloadData;
		unsigned long long buffer_size;
		if (client->download_to(REMOTEFILE, szTmpDownloadData, buffer_size))
		{
			jsFirmware.Parse(szTmpDownloadData);
			if (!WriteStringToFile(szTmpDataFile, jsFirmware.FastWrite()))
				return bRet;
		}
	} while (FALSE);
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
		m_WebDavOptions =
		{
		  {"webdav_hostname", jsFirmware.GetStringValue(WEBDAVURL)},
		  {"webdav_username", jsFirmware.GetStringValue(WEBDAVUSER)},
		  {"webdav_password", jsFirmware.GetStringValue(WEBDAVPASSWORD)}
		};
		m_bAutoSync = jsFirmware.GetIntValue(AUTOSYSNC);
	} while (FALSE);
	return true;
}