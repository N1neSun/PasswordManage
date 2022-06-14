#pragma once

#include "SQLiteCpp/SQLiteCpp.h"
#include "Sqlite3/sqlite3.h"
#include <vector>

struct TableColumnInfo
{
	std::string name;
	std::string type;
	std::string attribute;
};

struct PasswordColumnInfo
{
	PasswordColumnInfo()
	{
		id = -1;
		PasswordId = "";
		Name = "";
		Username = "";
		Password = "";
		Url = "";
		Notes = "";
		Isdelete = 0;
		GroupName = "default";
	}
	int id;
	std::string PasswordId;
	std::string Name;
	std::string Username;
	std::string Password;
	std::string Url;
	std::string Notes;
	int Isdelete;
	std::string GroupName;
};


class SqliteDatabase
{
protected:
	SqliteDatabase();
	virtual ~SqliteDatabase();

	bool InitDatabase();

public:
	bool InsertPasswordInfo(PasswordColumnInfo& info);

	bool UpdateControlInfo(PasswordColumnInfo& info);

	bool GetPasswordInfo(PasswordColumnInfo& control, const std::string& strPasswordId);

	bool GetGroupInfoList(std::vector<std::string>& vecGroupInfo);
	bool GetGroupListInfo(std::vector<PasswordColumnInfo*>& vecPasswordInfolList, const std::string& strGroupName);

	bool GetPasswordInfoList(std::vector<PasswordColumnInfo*>& vecPasswordInfolList);

	bool IsExist(const std::string& strName);

	bool RemovePasswordInfo(const PasswordColumnInfo& info);

	bool GetVersionInfo(std::string& strVersion);

	bool SetVersionInfo(const std::string& strVersion);

	bool GetSyncTimeInfo(unsigned int& uSyncTime);

	bool SetSyncTimeInfo(const unsigned int& uSyncTime);

	bool InitSyncInfo();

	bool SyncInfoIsExist();

	static SqliteDatabase& GetDBController();

private:
	SQLite::Database* db;
};