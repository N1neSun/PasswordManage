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
	std::vector<std::string> vecLocalJsonData;
	std::vector<std::string> vecRemoteJsonData;
	std::vector<std::string> vecSyncJsonData;

public:
	bool SqliteToJsonFile();
	bool JsonFileToSqlite();
	int DownloadRemoteJsonData();
	bool ReadSysncConfig();
	bool SyncJsonFile();
	bool PaserSyncFile();
	bool CompareSyncFile();
};