#include "util.h"

#include<openssl/aes.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>

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
