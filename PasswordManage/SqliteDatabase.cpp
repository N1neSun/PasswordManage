#include "SqliteDatabase.h"

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