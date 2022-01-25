#pragma once

#include "SQLiteCpp/SQLiteCpp.h"
#include "Sqlite3/sqlite3.h"
#include <vector>

struct TableColumnInfo
{
	std::string strName;
	std::string strUsername;
	std::string strPassword;
	std::string strUrl;
	std::string strNotes;
};

class SqliteDatabase
{

};