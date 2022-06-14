#pragma once
#include <map>
#include <string>
#include <webdav/client.hpp>

class SyncPassword {
public:
	SyncPassword();
	SyncPassword(std::map<std::string, std::string> WebDavOptions, int bAutoSync);
	~SyncPassword();

	std::map<std::string, std::string> m_WebDavOptions;
	int m_bAutoSync;
	std::string m_strSyncJsonVerison;
	unsigned int m_uSyncJsonTime;

public:
	bool SqliteToJsonFile();
	bool JsonFileToSqlite();
	int DownloadRemoteJsonData();
	bool ReadSysncConfig();
	bool SyncJsonFile();
};