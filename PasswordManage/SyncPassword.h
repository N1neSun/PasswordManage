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
	std::vector<std::string> m_vecLocalJsonData;
	std::vector<std::string> m_vecLocalJsonIndex;
	std::vector<std::string> m_vecRemoteJsonData;
	std::vector<std::string> m_vecRemoteJsonIndex;
	std::vector<std::string> m_vecSyncJsonData;

public:
	bool SqliteToJsonFile(BOOL bCopyFile);
	bool JsonFileToSqlite();
	int DownloadRemoteJsonData();
	bool ReadSysncConfig();
	bool SyncJsonFile();
	bool CompareSyncFile(const std::vector<std::string> vecSrc, const std::vector<std::string> vecDes);
};