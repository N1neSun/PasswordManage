#pragma once
#include <map>
#include <string>
#include <webdav/client.hpp>

class SyncPassword {
public:
	SyncPassword();
	SyncPassword(std::map<std::string, std::string> WebDavOptions, int bAutoSync, std::string strKey);
	~SyncPassword();
	std::string m_strKey;

	std::map<std::string, std::string> m_WebDavOptions;
	int m_bAutoSync;
	std::string m_strSyncJsonVerison;
	unsigned int m_uSyncJsonTime;
	std::string m_strSyncRemoteKey;
	std::vector<std::string> m_vecLocalJsonData;
	std::vector<std::string> m_vecLocalJsonIndex;
	std::vector<std::string> m_vecRemoteJsonData;
	std::vector<std::string> m_vecRemoteJsonIndex;
	std::vector<std::string> m_vecSyncJsonData;

public:
	bool SqliteToJsonFile(bool bCopyFile);
	bool JsonFileToSqlite();
	int DownloadRemoteJsonData();
	bool ReadSysncConfig();
	bool SyncJsonFile();
	bool CompareSyncFile(const std::vector<std::string> vecSrc, const std::vector<std::string> vecDes);
};