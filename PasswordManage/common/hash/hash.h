#ifndef _HASH_H_
#define _HASH_H_

#include <string>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#define MAX_HASH_BUFFER_SIZE 0x7fffffff

#define MD4_SIZE 16
#define MD4_STRING_SIZE 32

#define MD5_SIZE 16
#define MD5_STRING_SIZE 32

#define SHA1_SIZE 20
#define SHA1_STRING_SIZE 40

#define SHA256_SIZE 32
#define SHA256_STRING_SIZE 64

#define SHA384_SIZE 48
#define SHA384_STRING_SIZE 96

#define SHA512_SIZE 64
#define SHA512_STRING_SIZE 128


int hex_decode_digit(char c);
bool HexToString(std::string &strbuf, const unsigned char *src, size_t srclen);
bool HexToStringEx(char *dest, size_t destlen, const unsigned char *src, size_t srclen);
bool StringToHex(char *dest, size_t destlen, const char *src, size_t srclen);
// md4
bool md4_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen);
bool md4_buffer_string(const unsigned char *buf, size_t len, std::string &strHash);
bool md4_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen);
bool md4_file_string(const char *filepath, std::string &strHash);
bool md4_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen);
bool md4_file_string(const wchar_t *filepath, std::string &strHash);
// md5
bool md5_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen);
bool md5_buffer_string(const unsigned char *buf, size_t len, std::string &strHash);
bool md5_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen);
bool md5_file_string(const char *filepath, std::string &strHash);
bool md5_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen);
bool md5_file_string(const wchar_t *filepath, std::string &strHash);
// sha1
bool sha1_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen);
bool sha1_buffer_string(const unsigned char *buf, size_t len, std::string &strHash);
bool sha1_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen);
bool sha1_file_string(const char *filepath, std::string &strHash);
bool sha1_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen);
bool sha1_file_string(const wchar_t *filepath, std::string &strHash);
// sha256
bool sha256_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen);
bool sha256_buffer_string(const unsigned char *buf, size_t len, std::string &strHash);
bool sha256_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen);
bool sha256_file_string(const char *filepath, std::string &strHash);
bool sha256_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen);
bool sha256_file_string(const wchar_t *filepath, std::string &strHash);
// sha384
bool sha384_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen);
bool sha384_buffer_string(const unsigned char *buf, size_t len, std::string &strHash);
bool sha384_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen);
bool sha384_file_string(const char *filepath, std::string &strHash);
bool sha384_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen);
bool sha384_file_string(const wchar_t *filepath, std::string &strHash);
// sha512
bool sha512_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen);
bool sha512_buffer_string(const unsigned char *buf, size_t len, std::string &strHash);
bool sha512_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen);
bool sha512_file_string(const char *filepath, std::string &strHash);
bool sha512_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen);
bool sha512_file_string(const wchar_t *filepath, std::string &strHash);

//hash_cheack

typedef enum
{
	md4  = 0,
	md5,
	sha1,
	sha256,
	sha384,
	sha512,
}E_HASH_TYPE;


bool com_ss_hash(E_HASH_TYPE hashType, const unsigned char* psz1Buf, unsigned int u1len, 
				 const unsigned char* psz2Buf, unsigned int u2len);


bool com_sh_hash(E_HASH_TYPE hashType, const unsigned char* pszBuf, unsigned int ulen, 
				 const unsigned char* pszHash, unsigned int uHashlen);

bool com_ff_hash(E_HASH_TYPE hashType, char* p1file, char* p2file);


bool com_fh_hash(E_HASH_TYPE hashType, char* p1file, 
				 const unsigned char* pszHash, unsigned int uHashlen);

#endif