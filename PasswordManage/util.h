#pragma once
#include <windows.h>
#include <string>
#include <map>
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>


#define KEY_MAX_LEN 256
#define KEY_FILE "DecryptKey.key"
#define SYNCONFIG_FILE "SysncConfig"
#define SIGN "N1NESUN"
#define SYNCDATAFILE "SyncData"
#define SYNCTMPDATAFILE "SyncData.tmp"
#define SYNCVERSION "version"
#define SYNCTIME "time"
#define WEBDAVURL "WebDavUrl"
#define WEBDAVUSER "User"
#define WEBDAVPASSWORD "Password"
#define AUTOSYSNC "AutoSysnc"
#define REMOTEFILE "Password/SyncData"

typedef std::map<int, std::uniform_int_distribution<unsigned>> RandomTypeFuc;

std::string base64_encode(char const* bytes_to_encode, int in_len);

std::string base64_decode(std::string& encoded_string);

std::string aes_256_cbc_encode(const std::string& password, const std::string& data);

std::string aes_256_cbc_decode(const std::string& password, const std::string& strData);

void GetRandString(LPSTR szStr, DWORD len);

BOOL CreateUUID(char* szBuffer);

BOOL CopyStringToClipboard(const std::string strText);

BOOL LoadDecryptKey(const CHAR* pczPin /*= ""*/, std::string& strDecryptKey);

BOOL SaveDecryptKey(const CHAR* pczPin /*= ""*/, const std::string& strDecryptKey);

bool ReadFileToString(const char* pFilePath, std::string& strData);

bool WriteStringToFile(const char* pFilePath, const std::string& strData);

void GetRandomNum(std::string& strNum, int nCount);

void GetRandomLowerChar(std::string& strChar, int nCount);

void GetRandomUpperChar(std::string& strChar, int nCount);

void GetRandomSymbol(std::string& strSymbol, int nCount);

std::string GetRandomPassword(const std::string& strPassword, int nCount, int nTypeCount);

template<typename T>
std::vector<T> vector_distinct(std::vector<T> result);

template<typename T>
std::vector<T> vectorToset_distinct(std::vector<T> vec);