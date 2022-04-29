#pragma once
#include <windows.h>
#include <string>
#include <map>
#include<random>


#define KEY_MAX_LEN 256
#define KEY_FILE "DecryptKey.key"
#define SIGN "N1NESUN"

typedef std::map<int, std::uniform_int_distribution<unsigned>> RandomTypeFuc;

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