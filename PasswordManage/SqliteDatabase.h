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
		Name = "";
		Username = "";
		Password = "";
		Url = "";
		Notes = "";
	}
	int id;
	std::string Name;
	std::string Username;
	std::string Password;
	std::string Url;
	std::string Notes;
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

	bool GetPasswordInfo(PasswordColumnInfo& control, const std::string& strName);

	static SqliteDatabase& GetDBController();

private:
	SQLite::Database* db;
};