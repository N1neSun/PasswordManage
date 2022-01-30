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
		strName = "";
		strUsername = "";
		strPassword = "";
		strUrl = "";
		strNotes = "";
	}
	std::string strName;
	std::string strUsername;
	std::string strPassword;
	std::string strUrl;
	std::string strNotes;
};


class SqliteDatabase
{
protected:
	SqliteDatabase();
	virtual ~SqliteDatabase();

	bool InitDatabase();

private:
	SQLite::Database* db;
};