#include "SqliteDatabase.h"
#include <atlstr.h>
#include "util.h"

#define DATABASE "PasswordDB.db"
#define MASTER_TABLE "PasswordInfo"
#define VERSION_TABLE "VerisonInfo"

static struct TableColumnInfo PASSWORD_TABLE_COLUMNS[] = {
	{"id", "INTEGER", "NOT NULL PRIMARY KEY autoincrement"},
	{"PasswordId", "varchar(512)", "NOT NULL"},
	{"Name", "varchar(512)", "NOT NULL"},
	{"Username", "varchar(512)", "NOT NULL"},
	{"Password", "varchar(512)", "NOT NULL"},
	{"Url", "varchar(512)", ""},
	{"Notes", "text", ""},
	{"Isdelete", "int", ""},
	{"GroupName", "varchar(512)", ""}
};
static int PASSWORD_TABLE_COLUMNS_NUM = sizeof(PASSWORD_TABLE_COLUMNS) / sizeof(PASSWORD_TABLE_COLUMNS[0]);

static struct TableColumnInfo VERSION_TABLE_COLUMNS[] ={
	{"version", "varchar(512)", "NOT NULL"},
	{"synctime", "integer", ""}
};
static int VERSION_TABLE_COLUMNS_NUM = sizeof(VERSION_TABLE_COLUMNS) / sizeof(VERSION_TABLE_COLUMNS[0]);

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

	CStringA createVersionTableSQL;
	MakeCreateTableSQL(createVersionTableSQL, VERSION_TABLE, VERSION_TABLE_COLUMNS, VERSION_TABLE_COLUMNS_NUM);
	SQLite::Statement doCreateVersionTable(*db, createVersionTableSQL.GetBuffer());
	try
	{
		doCreateVersionTable.exec();
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		exit(0);
	}

	return true;
}

SqliteDatabase& SqliteDatabase::GetDBController()
{
	static SqliteDatabase db;
	return db;
}

bool SqliteDatabase::InsertPasswordInfo(PasswordColumnInfo& info)
{
	bool bRet = true;

	char szUUID[KEY_MAX_LEN] = { 0 };
	if (info.PasswordId == "")
	{
		CreateUUID(szUUID);
		info.PasswordId = szUUID;
	}
	CStringA insertControl;
	insertControl.Format("INSERT INTO %s(PasswordId,Name,Username,Password,Url,Notes,GroupName,Isdelete)VALUES(\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\", %d)",
		MASTER_TABLE, info.PasswordId.c_str(), info.Name.c_str(), info.Username.c_str(), info.Password.c_str(), info.Url.c_str(), info.Notes.c_str(), info.GroupName.c_str(), info.Isdelete);

	SQLite::Statement doInsertSQL(*db, insertControl.GetBuffer());

	try
	{
		doInsertSQL.exec();
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		bRet = false;
		exit(0);
	}

	return bRet;
}

bool SqliteDatabase::UpdateControlInfo(PasswordColumnInfo& info)
{
	bool bRet = true;

	CStringA updateControl;
	updateControl.Format("UPDATE %s SET Name=\"%s\", Username=\"%s\", Password=\"%s\", Url=\"%s\", Notes=\"%s\", GroupName=\"%s\"  WHERE PasswordId=\"%s\"",
		MASTER_TABLE, info.Name.c_str(), info.Username.c_str(), info.Password.c_str(), info.Url.c_str(), info.Notes.c_str(), info.GroupName.c_str(), info.PasswordId.c_str());
	SQLite::Statement doUpdateSQL(*db, updateControl.GetBuffer());

	try
	{
		doUpdateSQL.exec();
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		bRet = false;
		exit(0);
	}

	return bRet;
}

bool SqliteDatabase::GetPasswordInfo(PasswordColumnInfo& info, const std::string& strPasswordId)
{
	bool bRet = true;

	CStringA getControlSQL;
	getControlSQL.Format("SELECT * FROM %s where PasswordId =\"%s\"", MASTER_TABLE, strPasswordId.c_str());

	SQLite::Statement doGetSQL(*db, getControlSQL.GetBuffer());
	try
	{
		doGetSQL.executeStep();
		if (!doGetSQL.isDone())
		{
			info.id = doGetSQL.getColumn("id").getInt();
			info.PasswordId = doGetSQL.getColumn("PasswordId").getString();
			info.Name = doGetSQL.getColumn("Name").getString();
			info.Username = doGetSQL.getColumn("Username").getInt();
			info.Password = doGetSQL.getColumn("Password").getString();
			info.Url = doGetSQL.getColumn("Url").getString();
			info.Notes = doGetSQL.getColumn("Notes").getString();
			info.GroupName = doGetSQL.getColumn("GroupName").getString();
		}
		else
		{
			bRet = false;
		}
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		bRet = false;
		exit(0);
	}

	return bRet;
}

bool SqliteDatabase::GetGroupListInfo(std::vector<PasswordColumnInfo*>& vecPasswordInfolList, const std::string& strGroupName)
{
	bool bRet = true;

	CStringA getControlSQL;
	getControlSQL.Format("SELECT * FROM %s where GroupName =\"%s\" and Isdelete=0", MASTER_TABLE, strGroupName.c_str());

	SQLite::Statement doGetSQL(*db, getControlSQL.GetBuffer());
	while (true)
	{
		if (doGetSQL.executeStep())
		{
			if (doGetSQL.isDone())
				break;

			PasswordColumnInfo* info = new PasswordColumnInfo();

			info->id = doGetSQL.getColumn("id").getInt();
			info->PasswordId = doGetSQL.getColumn("PasswordId").getString();
			info->Name = doGetSQL.getColumn("Name").getString();
			info->Username = doGetSQL.getColumn("Username").getString();
			info->Password = doGetSQL.getColumn("Password").getString();
			info->Url = doGetSQL.getColumn("Url").getString();
			info->Notes = doGetSQL.getColumn("Notes").getString();
			info->GroupName = doGetSQL.getColumn("GroupName").getString();

			vecPasswordInfolList.push_back(info);
		}
		else
		{
			break;
		}

	}


	return true;
}

bool SqliteDatabase::GetGroupInfoList(std::vector<std::string>& vecGroupInfo)
{
	CStringA getControlSQL;
	getControlSQL.Format("SELECT * FROM %s where Isdelete=0 Group by GroupName", MASTER_TABLE);

	SQLite::Statement doGetSQL(*db, getControlSQL.GetBuffer());

	while (true)
	{
		if (doGetSQL.executeStep())
		{
			if (doGetSQL.isDone())
				break;

			vecGroupInfo.push_back(doGetSQL.getColumn("GroupName").getString());
		}
		else
		{
			break;
		}

	}

	return true;
}

bool SqliteDatabase::GetPasswordInfoList(std::vector<PasswordColumnInfo*>& vecPasswordInfolList)
{
	CStringA getControlSQL;
	getControlSQL.Format("SELECT * FROM %s where Isdelete=0", MASTER_TABLE);

	SQLite::Statement doGetSQL(*db, getControlSQL.GetBuffer());
	while(true)
	{
		if (doGetSQL.executeStep())
		{
			if (doGetSQL.isDone())
				break;

			PasswordColumnInfo* info = new PasswordColumnInfo();

			info->id = doGetSQL.getColumn("id").getInt();
			info->PasswordId = doGetSQL.getColumn("PasswordId").getString();
			info->Name = doGetSQL.getColumn("Name").getString();
			info->Username = doGetSQL.getColumn("Username").getString();
			info->Password = doGetSQL.getColumn("Password").getString();
			info->Url = doGetSQL.getColumn("Url").getString();
			info->Notes = doGetSQL.getColumn("Notes").getString();
			info->GroupName = doGetSQL.getColumn("GroupName").getString();

			vecPasswordInfolList.push_back(info);
		}
		else
		{
			break;
		}

	}


	return true;
}

bool SqliteDatabase::IsExist(const std::string& strPasswordId)
{
	bool bRet = true;

	CStringA getControlSQL;
	getControlSQL.Format("SELECT * FROM %s where PasswordId =\"%s\"", MASTER_TABLE, strPasswordId.c_str());

	SQLite::Statement doGetSQL(*db, getControlSQL.GetBuffer());
	try
	{
		doGetSQL.executeStep();
		if (!doGetSQL.isDone())
		{
			if (doGetSQL.getColumn("PasswordId").getString() == strPasswordId)
			{
				bRet = true;
			}
			else
			{
				bRet = false;
			}
		}
		else
		{
			bRet = false;
		}
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		bRet = false;
		exit(0);
	}

	return bRet;
}

bool SqliteDatabase::RemovePasswordInfo(const PasswordColumnInfo& info)
{
	CStringA deleteSQL;
	deleteSQL.Format("UPDATE %s SET Isdelete=%d WHERE PasswordId = \"%s\"", MASTER_TABLE, info.Isdelete, info.PasswordId.c_str());
	SQLite::Statement doDeleteSQL(*db, deleteSQL.GetBuffer());
	try
	{
		doDeleteSQL.exec();
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		exit(0);
	}

	return true;
}

bool SqliteDatabase::GetVersionInfo(std::string& strVersion)
{
	CStringA getControlSQL;
	getControlSQL.Format("SELECT * FROM %s", VERSION_TABLE);

	SQLite::Statement doGetSQL(*db, getControlSQL.GetBuffer());

	while (true)
	{
		if (doGetSQL.executeStep())
		{
			if (doGetSQL.isDone())
				break;

			strVersion = doGetSQL.getColumn("version").getString();
		}
		else
		{
			break;
		}

	}

	return true;
}

bool SqliteDatabase::SetVersionInfo(const std::string& strVersion)
{
	CStringA setControlSQL;
	setControlSQL.Format("UPDATE %s SET version=%s", VERSION_TABLE, strVersion.c_str());

	SQLite::Statement doSetSQL(*db, setControlSQL.GetBuffer());
	try
	{
		doSetSQL.exec();
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		exit(0);
	}

	return true;
}


bool SqliteDatabase::GetSyncTimeInfo(unsigned int& strSyncTime)
{
	CStringA getControlSQL;
	getControlSQL.Format("SELECT * FROM %s", VERSION_TABLE);

	SQLite::Statement doGetSQL(*db, getControlSQL.GetBuffer());

	while (true)
	{
		if (doGetSQL.executeStep())
		{
			if (doGetSQL.isDone())
				break;

			strSyncTime = doGetSQL.getColumn("sysnctime").getUInt();
		}
		else
		{
			break;
		}

	}

	return true;
}

bool SqliteDatabase::SetSyncTimeInfo(const unsigned int& strSyncTime)
{
	CStringA setControlSQL;
	setControlSQL.Format("UPDATE %s SET sysnctime=%d", VERSION_TABLE, strSyncTime);

	SQLite::Statement doSetSQL(*db, setControlSQL.GetBuffer());
	try
	{
		doSetSQL.exec();
	}
	catch (const SQLite::Exception&)
	{
		assert(false);
		exit(0);
	}

	return true;
}