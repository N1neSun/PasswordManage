#include "util.h"

#include<openssl/aes.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include<atlstr.h>

#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")

std::string aes_256_cbc_encode(const std::string& password, const std::string& data)
{
	unsigned char iv[AES_BLOCK_SIZE] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };

	AES_KEY aes_key;
	if (AES_set_encrypt_key((const unsigned char*)password.c_str(), password.length() * 8, &aes_key) < 0)
	{
		//assert(false);
		return "";
	}
	std::string strRet;
	std::string data_bak = data;
	unsigned int data_length = data_bak.length();

	// ZeroPadding
	int padding = 0;
	if (data_bak.length() % (AES_BLOCK_SIZE) > 0)
	{
		padding = AES_BLOCK_SIZE - data_bak.length() % (AES_BLOCK_SIZE);
	}


	data_length += padding;
	while (padding > 0)
	{
		data_bak += '\0';
		padding--;
	}

	for (unsigned int i = 0; i < data_length / (AES_BLOCK_SIZE); i++)
	{
		std::string str16 = data_bak.substr(i * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		unsigned char out[AES_BLOCK_SIZE];
		::memset(out, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt((const unsigned char*)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);
		strRet += std::string((const char*)out, AES_BLOCK_SIZE);
	}
	return strRet;
}

std::string aes_256_cbc_decode(const std::string& password, const std::string& strData)
{

	unsigned char iv[AES_BLOCK_SIZE] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };

	AES_KEY aes_key;
	if (AES_set_decrypt_key((const unsigned char*)password.c_str(), password.length() * 8, &aes_key) < 0)
	{
		return "";
	}

	std::string strRet;
	for (unsigned int i = 0; i < strData.length() / AES_BLOCK_SIZE; i++)
	{
		std::string str16 = strData.substr(i * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		unsigned char out[AES_BLOCK_SIZE];
		::memset(out, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt((const unsigned char*)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_DECRYPT);
		strRet += std::string((const char*)out, AES_BLOCK_SIZE);
	}
	return strRet;
}

void GetRandString(LPSTR szStr, DWORD len)
{
	if (szStr == NULL)
		return;
	char* pvCode = szStr;
	char  singleCode[2];
	char  verificationValue[64] = "00123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	memset(szStr, 0, len);
	memset(singleCode, 0, 2);
	srand((unsigned int)time((time_t*)NULL));
	for (unsigned int i = 1; i <= len; ++i)
	{
		sprintf(singleCode, "%c", verificationValue[(rand() % 62) + 1]);
		strcpy(szStr, singleCode);
	}

}

BOOL CreateUUID(char* szBuffer)
{
	char szTempBuffer[KEY_MAX_LEN] = { 0 };
	GUID guid;
	if (CoCreateGuid(&guid))
	{
		return FALSE;
	}
	sprintf(szTempBuffer, "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1]
		, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	memcpy(szBuffer, szTempBuffer, KEY_MAX_LEN * sizeof(char));
	return TRUE;
}


BOOL CopyStringToClipboard(const std::string strText)
{
	if (!::OpenClipboard(NULL))
	{
		return FALSE;
	}

	CString src = strText.c_str();
	::EmptyClipboard();
	size_t cbStr = (src.GetLength() + 1) * sizeof(TCHAR);
	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);
	memcpy_s(GlobalLock(hData), cbStr, src.LockBuffer(), cbStr);
	::GlobalUnlock(hData);
	src.UnlockBuffer();
	::SetClipboardData(CF_TEXT, hData);
	::CloseClipboard();
	return TRUE;
}

BOOL LoadDecryptKey(const CHAR* pczPin /*= ""*/, std::string& strDecryptKey)
{
	BOOL bRet = FALSE;

	do
	{
		TCHAR szFirmware[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szFirmware, MAX_PATH);
		PathRemoveFileSpec(szFirmware);
		PathAppend(szFirmware, KEY_FILE);

		if (!PathFileExists(szFirmware))
		{
			break;
		}

		std::string strData = "";
		if (!ReadFileToString(szFirmware, strData))
		{
			break;
		}


		std::string strDecrypt = aes_256_cbc_decode(pczPin, strData);
		if (strDecrypt.empty() || (strDecrypt.find(SIGN) != 0))
		{
			break;
		}
		strDecryptKey = strDecrypt.c_str();
		
		bRet = TRUE;

	} while (FALSE);

	return bRet;
}

BOOL SaveDecryptKey(const CHAR* pczPin /*= ""*/, const std::string& strDecryptKey)
{
	BOOL bRet = FALSE;

	do
	{
		TCHAR szKeyFile[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szKeyFile, MAX_PATH);
		PathRemoveFileSpec(szKeyFile);
		PathAppend(szKeyFile, KEY_FILE);

		if (!PathFileExists(szKeyFile))
		{
			break;
		}

		std::string strEncrypt = aes_256_cbc_encode(pczPin, strDecryptKey);
		if (strEncrypt.empty())
		{
			break;
		}

		if (!WriteStringToFile(szKeyFile, strEncrypt))
		{
			break;
		}
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

bool ReadFileToString(const char* pFilePath, std::string& strData)
{
	FILE* pFile = NULL;
	char* buf = NULL;
	int len = 0;
	bool bRet = false;
	strData.clear();
	pFile = fopen(pFilePath, "rb");
	if (NULL == pFile)
	{
		return false;
	}

	fseek(pFile, 0, SEEK_END);
	len = ftell(pFile);
	rewind(pFile);

	buf = new char[len];
	if (NULL == buf)
	{
		fclose(pFile);
		return false;
	}

	bRet = (1 == fread(buf, len, 1, pFile));
	fclose(pFile);
	if (bRet)
	{
		strData.assign(buf, len);
	}
	delete[] buf;
	return bRet;
}

bool WriteStringToFile(const char* pFilePath, const std::string& strData)
{
	FILE* pFile = NULL;
	char* buf = NULL;
	bool bRet = false;
	pFile = fopen(pFilePath, "wb");
	if (NULL == pFile)
	{
		return false;
	}
	bRet = (1 == fwrite(strData.c_str(), strData.size(), 1, pFile));
	fclose(pFile);
	return bRet;
}

void GetRandomNum(std::string& strNum, int nCount)
{
	strNum.clear();
	char  singleCode[2];
	char  verificationValue[11] = "0123456789";
	memset(singleCode, 0, 2);
	srand((unsigned int)time((time_t*)NULL));
	for (unsigned int i = 1; i <= nCount; ++i)
	{
		sprintf(singleCode, "%c", verificationValue[(rand() % 10)]);
		strNum += singleCode;
	}
}

void GetRandomLowerChar(std::string& strChar, int nCount)
{
	strChar.clear();
	char  singleCode[2];
	char  verificationValue[27] = "abcdefghijklmnopqrstuvwxyz";
	memset(singleCode, 0, 2);
	srand((unsigned int)time((time_t*)NULL));
	for (unsigned int i = 1; i <= nCount; ++i)
	{
		sprintf(singleCode, "%c", verificationValue[(rand() % 26)]);
		strChar += singleCode;
	}
}

void GetRandomUpperChar(std::string& strChar, int nCount)
{
	strChar.clear();
	char  singleCode[2];
	char  verificationValue[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	memset(singleCode, 0, 2);
	srand((unsigned int)time((time_t*)NULL));
	for (unsigned int i = 1; i <= nCount; ++i)
	{
		sprintf(singleCode, "%c", verificationValue[(rand() % 26)]);
		strChar += singleCode;
	}
}

void GetRandomSymbol(std::string& strSymbol, int nCount)
{
	strSymbol.clear();
	char  singleCode[2];
	char  verificationValue[29] = "`~!@#$%^&*()_-+={}|[]:;,.<>?";
	memset(singleCode, 0, 2);
	srand((unsigned int)time((time_t*)NULL));
	for (unsigned int i = 1; i <= nCount; ++i)
	{
		sprintf(singleCode, "%c", verificationValue[(rand() % 28)]);
		strSymbol += singleCode;
	}
}

std::string GetRandomPassword(const std::string& strPassword, int nCount)
{
	std::string strRandomPassword;
	char  singleCode[2];
	memset(singleCode, 0, 2);
	srand((unsigned int)time((time_t*)NULL));
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		sprintf(singleCode, "%c", strPassword[(rand() % strPassword.length())]);
		strRandomPassword += singleCode;
	}
	return strRandomPassword;
}