#include "SqliteDatabase.h"
#include <atlstr.h>

#define DATABASE "PasswordDB.db"
#define MASTER_TABLE "PasswordInfo"

static struct TableColumnInfo PASSWORD_TABLE_COLUMNS[] = {
	{"id", "int", "NOT NULL PRIMARY KEY"},
	{"strName", "varchar(512)", "NOT NULL PRIMARY KEY"},
	{"strUsername", "varchar(512)", "NOT NULL"},
	{"strPassword", "varchar(512)", "NOT NULL"},
	{"strUrl", "varchar(512)", "NOT NULL DEFAULT 100"},
	{"strNotes", "text", "NOT NULL"}
};
static int PASSWORD_TABLE_COLUMNS_NUM = sizeof(PASSWORD_TABLE_COLUMNS) / sizeof(PASSWORD_TABLE_COLUMNS[0]);

SqliteDatabase::SqliteDatabase()
{
	db = NULL;
	InitDatabase();
}

SqliteDatabase::~SqliteDatabase()
{
	if (db)
		delete db;
}

static void MakeCreateTableSQL(CStringA& sql, const char* tableName, const TableColumnInfo columns[], int columnsLen)
{
	sql.Empty();

	sql = "CREATE TABLE IF NOT EXISTS ";
	sql += tableName;
	sql += "(";
	for (int i = 0; i < columnsLen; i++)
	{
		if (i > 0)
			sql += ",";

		sql += columns[i].name.c_str();
		sql += " ";
		sql += columns[i].type.c_str();
		sql += " ";
		sql += columns[i].attribute.c_str();
	}
	sql += ")";
}

bool SqliteDatabase::InitDatabase()
{
	// create db
	try
	{
		db = new SQLite::Database(DATABASE, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		exit(0);
	}

	// create tables
	CStringA createTableSQL;
	MakeCreateTableSQL(createTableSQL, MASTER_TABLE, PASSWORD_TABLE_COLUMNS, PASSWORD_TABLE_COLUMNS_NUM);
	SQLite::Statement doCreateTable(*db, createTableSQL.GetBuffer());
	try
	{
		doCreateTable.exec();
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		exit(0);
	}

	return true;
}