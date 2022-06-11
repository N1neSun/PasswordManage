#pragma once
#include <map>
#include <string>

class SyncPassword {
public:
	SyncPassword();
	~SyncPassword();

	std::map<std::string, std::string> m_WebDavOptions;
	BOOL m_bAutoSync;
public:
	bool SqliteToJsonFile();
	bool JsonFileToSqlite();
	bool DownloadRemoteJsonData();
	bool ReadSysncConfig();
};