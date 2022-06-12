#pragma once
#include <map>
#include <string>

class SyncPassword {
public:
	SyncPassword();
	~SyncPassword();

	std::map<std::string, std::string> m_WebDavOptions;
	BOOL m_bAutoSync;
	std::string m_strSyncJsonVerison;
	unsigned int m_uSyncJsonTime;

public:
	bool SqliteToJsonFile();
	bool JsonFileToSqlite();
	bool DownloadRemoteJsonData();
	bool ReadSysncConfig();
	bool SyncJsonFile();
};