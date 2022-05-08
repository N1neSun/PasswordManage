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

void SyncPassword::SqliteToJsonFile()
{
	TCHAR szKeyFile[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szKeyFile, MAX_PATH);
	PathRemoveFileSpec(szKeyFile);
	PathAppend(szKeyFile, SYNCDATAFILE);
	CJson jsFirmware;
	std::vector<PasswordColumnInfo*> vecPasswordInfo;
	SqliteDatabase::GetDBController().GetPasswordInfoList(vecPasswordInfo);
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
	jsFirmware.AddValue("key", strSyncDataMd5);

	WriteStringToFile(szKeyFile, jsFirmware.FastWrite());
}