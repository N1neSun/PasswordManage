#pragma once
#include <windows.h>
#include <string>


#define KEY_MAX_LEN 256
#define KEY_FILE "DecryptKey.key"

std::string aes_256_cbc_encode(const std::string& password, const std::string& data);

std::string aes_256_cbc_decode(const std::string& password, const std::string& strData);

void GetRandString(LPSTR szStr, DWORD len);

BOOL CreateUUID(char* szBuffer);

BOOL CopyStringToClipboard(const std::string strText);

BOOL LoadDecryptKey(const CHAR* pczPin /*= ""*/, std::string& strDecryptKey);

BOOL SaveDecryptKey(const std::string& pczPin /*= ""*/, const std::string& strDecryptKey);

bool ReadFileToString(const char* pFilePath, std::string& strData);

bool WriteStringToFile(const char* pFilePath, const std::string& strData);