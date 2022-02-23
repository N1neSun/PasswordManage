#pragma once
#include <windows.h>
#include <string>

#define KEY_MAX_LEN 256

std::string aes_256_cbc_encode(const std::string& password, const std::string& data);

std::string aes_256_cbc_decode(const std::string& password, const std::string& strData);

void GetRandString(LPSTR szStr, DWORD len);

BOOL CreateUUID(char* szBuffer);