#pragma once
class SyncPassword {
public:
	SyncPassword();
	~SyncPassword();
public:
	bool SqliteToJsonFile();
	bool JsonFileToSqlite();
	bool DownloadJsonFile();
};