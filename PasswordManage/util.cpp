#include "util.h"

#include<openssl/aes.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include<atlstr.h>
#include "openssl/sha.h"
#include <openssl/rc4.h>

#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")

static const std::string base64_chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(char const* bytes_to_encode, int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;

}

std::string base64_decode(std::string& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}


char* base64_decode_block(char* inData)
{
	if (NULL == inData)
	{
		return NULL;
	}

	int inl, outl, blocksize;

	inl = strlen(inData);
	blocksize = inl * 6 / 8;

	char buffer[1000];
	memset(buffer, 0, blocksize);

	outl = EVP_DecodeBlock((unsigned char*)buffer, (unsigned char*)inData, inl);

	return _strdup(buffer);
}

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

void RC4_Decode(const std::string& strKey, const unsigned char* szData, unsigned char* szDecryptData)
{
	RC4_KEY key;
	RC4_set_key(&key, strKey.length(), (unsigned char*)strKey.c_str());
	RC4(&key, strlen((char*)szData), szData, szDecryptData);
}

void RC4_Decode(const unsigned char* szKey, const unsigned char* szData, unsigned char* szDecryptData)
{
	RC4_KEY key;
	RC4_set_key(&key, 32, szKey);
	RC4(&key, strlen((char*)szData), szData, szDecryptData);
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

std::string GetRandomPassword(const std::string& strPassword, int nCount, int nTypeCount)
{
	std::string strRandomPassword;
	char  singleCode[2];
	memset(singleCode, 0, 2);
	//srand(time(0));
	static std::default_random_engine generator;
	RandomTypeFuc distribution;
	int nMin = 0;
	int nMax = strPassword.length() / nTypeCount;
	int nLen = strPassword.length() / nTypeCount;
	for (int nIndex = 0; nIndex < nTypeCount; nIndex++)
	{
		std::uniform_int_distribution<unsigned>  distributionType(nMin, nMax-1);
		distribution[nIndex] = distributionType;
		nMin += nLen;
		nMax += nLen;
	}
	
	static std::normal_distribution<double> distributionType(0, nTypeCount);
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		int nType = abs(distributionType(generator));
		int nRand = distribution[nType](generator);
		sprintf(singleCode, "%c", strPassword[(nRand % strPassword.length())]);
		strRandomPassword += singleCode;
	}
	return strRandomPassword;
}

std::string GetUsernameAndSid(bool bReverse)
{
	char userName[MAX_PATH] = "";
	char sid[MAX_PATH] = "";
	DWORD nameSize = sizeof(userName);
	GetUserName(userName, &nameSize);


	char userSID[MAX_PATH] = "";
	char userDomain[MAX_PATH] = "";
	DWORD sidSize = sizeof(userSID);
	DWORD domainSize = sizeof(userDomain);


	SID_NAME_USE snu;
	LookupAccountName(NULL,
		(LPSTR)userName,
		(PSID)userSID,
		&sidSize,
		(LPSTR)userDomain,
		&domainSize,
		&snu);


	PSID_IDENTIFIER_AUTHORITY psia = GetSidIdentifierAuthority(userSID);
	sidSize = sprintf(sid, "S-%lu-", SID_REVISION);
	sidSize += sprintf(sid + strlen(sid), "%-lu", psia->Value[5]);


	int i = 0;
	int subAuthorities = *GetSidSubAuthorityCount(userSID);


	for (i = 0; i < subAuthorities; i++)
	{
		sidSize += sprintf(sid + sidSize, "-%lu", *GetSidSubAuthority(userSID, i));
	}
	char szBuff[1024];
	if(bReverse){
		_strrev(userName);
	}
	sprintf(szBuff, "%.*s%.*s", sizeof(userName), userName, sizeof(sid), sid);
	if (bReverse) {
		_strrev(szBuff);
	}
	return szBuff;
}

std::string getNchar(std::string str, int n)
{
	std::string nChars;


	if (n > str.size())
	{
		n = str.size();
	}

	for (size_t i = 0; i < n; i++)
	{
		nChars.push_back(str[i]);
	}

	return nChars;
}
