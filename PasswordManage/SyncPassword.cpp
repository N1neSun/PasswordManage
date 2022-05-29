#include "SyncPassword.h"
#include "SqliteDatabase.h"
#include "JsonObject.h"
#include "hash.h"
#include "util.h"
#include<atlstr.h>

SyncPassword::SyncPassword()
{

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
		time_t time1 = time(0);
		jsFirmware.AddValue(SYNCTIME, (long)time1);

		if (!WriteStringToFile(szKeyFile, jsFirmware.FastWrite()))
			return bRet;
		SqliteDatabase::GetDBController().SetVersionInfo(strSyncDataMd5);
	} while (FALSE);
	return true;
}

bool SyncPassword::JsonFileToSqlite()
{
	bool bRet = false;
	std::string strJsonVersionIndb;
	SqliteDatabase::GetDBController().GetVersionInfo(strJsonVersionIndb);
	do
	{
		TCHAR szKeyFile[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szKeyFile, MAX_PATH);
		PathRemoveFileSpec(szKeyFile);
		PathAppend(szKeyFile, SYNCDATAFILE);
		CJson jsFirmware;
		std::string strJsonData;
		std::vector<std::string> vecTmpJsonData;
		if (!ReadFileToString(szKeyFile, strJsonData))
			return bRet;
		jsFirmware.Parse(strJsonData.c_str());
		std::string strJsonVersion = jsFirmware.GetStringValue(SYNCVERSION);
		if (strJsonVersionIndb == strJsonVersion)
			return true;
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