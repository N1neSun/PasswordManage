#include "hash.h"

#pragma warning(disable:4996)

#ifdef _WIN32
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")
#else

#endif

int hex_decode_digit(char c)
{
	switch (c) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'A': case 'a': return 10;
	case 'B': case 'b': return 11;
	case 'C': case 'c': return 12;
	case 'D': case 'd': return 13;
	case 'E': case 'e': return 14;
	case 'F': case 'f': return 15;
	default:
		return -1;
	}
}

bool HexToStringEx(char *dest, size_t destlen, const unsigned char *src, size_t srclen)
{
	const unsigned char *end;
	char *cp;

	if(destlen > MAX_HASH_BUFFER_SIZE || destlen < srclen*2+1)
	{
		return false;
	}

	cp = dest;
	end = src+srclen;
	while (src<end) {
		*cp++ = "0123456789ABCDEF"[ (*src) >> 4 ];
		*cp++ = "0123456789ABCDEF"[ (*src) & 0xf ];
		++src;
	}
	*cp = '\0';
	return true;
}


bool HexToString(std::string &strbuf, const unsigned char *src, size_t srclen)
{
	bool bRet = false;
	char *dest = NULL;
	size_t destlen = 0;
	if (NULL == src || 0 == srclen || MAX_HASH_BUFFER_SIZE < srclen)
	{
		return false;
	}
	do 
	{
		destlen = (srclen + 1) * 2;
		dest = new char[destlen];
		if (NULL == dest)
		{
			break;
		}
		memset(dest, 0, destlen);
		if (HexToStringEx(dest, destlen, src, srclen))
		{
			strbuf.assign(dest, srclen*2);
			bRet = true;
		}
	} while (0);

	if (dest)
	{
		delete[] dest;
		dest = NULL;
	}

	return bRet;

}
bool StringToHex(unsigned char *dest, size_t destlen, const char *src, size_t srclen)
{
	const char *end;

	int v1,v2;
	if ((srclen % 2) != 0)
	{
		return false;
	}
	if (destlen < srclen/2 || destlen > MAX_HASH_BUFFER_SIZE)
	{
		return false;
	}
	end = src+srclen;
	while (src<end) 
	{
		v1 = hex_decode_digit(*src);
		v2 = hex_decode_digit(*(src+1));
		if (v1<0||v2<0)
			return false;
		*(unsigned char*)dest = (v1<<4)|v2;
		++dest;
		src+=2;
	}
	return true;
}

// md4
bool md4_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen)
{
	if (NULL == buf || 0 == len || MAX_HASH_BUFFER_SIZE < len || 
		NULL == hashbuf || 0 == hashlen || MD4_SIZE > hashlen)
	{
		return false;
	}
	memset(hashbuf, 0, hashlen);
	MD4(buf, len, hashbuf);
	return true;
}

bool md4_buffer_string(const unsigned char *buf, size_t len, std::string &strHash)
{
	unsigned char hashbuf[MD4_SIZE] = {0};
	strHash.clear();
	if (md4_buffer_byte(buf, len, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, MD4_SIZE);
	}
	return false;
}

bool md4_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen)
{
	bool bRet = false;
	unsigned char *pFileData = NULL;
	unsigned int nFileSize = 0;
	struct stat fs = {0};
	int fd = -1;
	if (NULL == filepath || NULL == hashbuf || 0 == hashlen || MD4_SIZE > hashlen)
	{
		return false;
	}

	do 
	{
		if (0 != stat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
		{
			break;
		}
		fd = open(filepath, O_RDONLY|O_BINARY, NULL);
		if (fd < 0)
		{
			break;
		}

		nFileSize = fs.st_size;
		pFileData = new unsigned char[nFileSize];
		if (NULL == pFileData)
		{
			break;
		}
		if (nFileSize != read(fd, pFileData, nFileSize))
		{
			break;
		}
		bRet = md4_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
	} while (0);

	if (fd > 0)
	{
		close(fd);
	}
	if (pFileData)
	{
		delete[] pFileData;
		pFileData = NULL;
	}
	return bRet;
}

bool md4_file_string(const char *filepath, std::string &strHash)
{
	unsigned char hashbuf[MD4_SIZE] = {0};
	strHash.clear();
	if (md4_file_byte(filepath, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, MD4_SIZE);
	}
	return false;
}



bool md4_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen)
{
    bool bRet = false;
    unsigned char *pFileData = NULL;
    unsigned int nFileSize = 0;
    struct _stat64i32 fs = {0};
    int fd = -1;
    if (NULL == filepath || NULL == hashbuf || 0 == hashlen || MD4_SIZE > hashlen)
    {
        return false;
    }

    do 
    {
        if (0 != _wstat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
        {
            break;
        }
        fd = _wopen(filepath, O_RDONLY|O_BINARY, NULL);
        if (fd < 0)
        {
            break;
        }

        nFileSize = fs.st_size;
        pFileData = new unsigned char[nFileSize];
        if (NULL == pFileData)
        {
            break;
        }
        if (nFileSize != read(fd, pFileData, nFileSize))
        {
            break;
        }
        bRet = md4_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
    } while (0);

    if (fd > 0)
    {
        close(fd);
    }
    if (pFileData)
    {
        delete[] pFileData;
        pFileData = NULL;
    }
    return bRet;
}

bool md4_file_string(const wchar_t *filepath, std::string &strHash)
{
    unsigned char hashbuf[MD4_SIZE] = {0};
    strHash.clear();
    if (md4_file_byte(filepath, hashbuf, _countof(hashbuf)))
    {
        return HexToString(strHash, hashbuf, MD4_SIZE);
    }
    return false;
}

// md5
bool md5_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen)
{
	if (NULL == buf || 0 == len || MAX_HASH_BUFFER_SIZE < len || 
		NULL == hashbuf || 0 == hashlen || MD5_SIZE > hashlen)
	{
		return false;
	}
	memset(hashbuf, 0, hashlen);
	MD5(buf, len, hashbuf);
	return true;
}

bool md5_buffer_string(const unsigned char *buf, size_t len, std::string &strHash)
{
	unsigned char hashbuf[MD5_SIZE] = {0};
	strHash.clear();
	if (md5_buffer_byte(buf, len, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, MD5_SIZE);
	}
	return false;
}

bool md5_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen)
{
	bool bRet = false;
	unsigned char *pFileData = NULL;
	unsigned int nFileSize = 0;
	struct stat fs = {0};
	int fd = -1;
	if (NULL == filepath || NULL == hashbuf || 0 == hashlen || MD5_SIZE > hashlen)
	{
		return false;
	}

	do 
	{
		if (0 != stat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
		{
			break;
		}
		fd = open(filepath, O_RDONLY|O_BINARY, NULL);
		if (fd < 0)
		{
			break;
		}

		nFileSize = fs.st_size;
		pFileData = new unsigned char[nFileSize];
		if (NULL == pFileData)
		{
			break;
		}
		if (nFileSize != read(fd, pFileData, nFileSize))
		{
			break;
		}
		bRet = md5_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
	} while (0);

	if (fd > 0)
	{
		close(fd);
	}
	if (pFileData)
	{
		delete[] pFileData;
		pFileData = NULL;
	}
	return bRet;
}

bool md5_file_string(const char *filepath, std::string &strHash)
{
	unsigned char hashbuf[MD5_SIZE] = {0};
	strHash.clear();
	if (md5_file_byte(filepath, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, MD5_SIZE);
	}
	return false;
}

bool md5_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen)
{
    bool bRet = false;
    unsigned char *pFileData = NULL;
    unsigned int nFileSize = 0;
    struct _stat64i32 fs = {0};
    int fd = -1;
    if (NULL == filepath || NULL == hashbuf || 0 == hashlen || MD5_SIZE > hashlen)
    {
        return false;
    }

    do 
    {
        if (0 != _wstat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
        {
            break;
        }
        fd = _wopen(filepath, O_RDONLY|O_BINARY, NULL);
        if (fd < 0)
        {
            break;
        }

        nFileSize = fs.st_size;
        pFileData = new unsigned char[nFileSize];
        if (NULL == pFileData)
        {
            break;
        }
        if (nFileSize != read(fd, pFileData, nFileSize))
        {
            break;
        }
        bRet = md5_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
    } while (0);

    if (fd > 0)
    {
        close(fd);
    }
    if (pFileData)
    {
        delete[] pFileData;
        pFileData = NULL;
    }
    return bRet;
}

bool md5_file_string(const wchar_t *filepath, std::string &strHash)
{
    unsigned char hashbuf[MD5_SIZE] = {0};
    strHash.clear();
    if (md5_file_byte(filepath, hashbuf, _countof(hashbuf)))
    {
        return HexToString(strHash, hashbuf, MD5_SIZE);
    }
    return false;
}

// sha1
bool sha1_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen)
{
	if (NULL == buf || 0 == len || MAX_HASH_BUFFER_SIZE < len || 
		NULL == hashbuf || 0 == hashlen || SHA1_SIZE > hashlen)
	{
		return false;
	}
	memset(hashbuf, 0, hashlen);
	SHA1(buf, len, hashbuf);
	return true;
}

bool sha1_buffer_string(const unsigned char *buf, size_t len, std::string &strHash)
{
	unsigned char hashbuf[SHA1_SIZE] = {0};
	strHash.clear();
	if (sha1_buffer_byte(buf, len, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, SHA1_SIZE);
	}
	return false;
}

bool sha1_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen)
{
	bool bRet = false;
	unsigned char *pFileData = NULL;
	unsigned int nFileSize = 0;
	struct stat fs = {0};
	int fd = -1;
	if (NULL == filepath || NULL == hashbuf || 0 == hashlen || SHA1_SIZE > hashlen)
	{
		return false;
	}

	do 
	{
		if (0 != stat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
		{
			break;
		}
		fd = open(filepath, O_RDONLY|O_BINARY, NULL);
		if (fd < 0)
		{
			break;
		}

		nFileSize = fs.st_size;
		pFileData = new unsigned char[nFileSize];
		if (NULL == pFileData)
		{
			break;
		}
		if (nFileSize != read(fd, pFileData, nFileSize))
		{
			break;
		}
		bRet = sha1_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
	} while (0);

	if (fd > 0)
	{
		close(fd);
	}
	if (pFileData)
	{
		delete[] pFileData;
		pFileData = NULL;
	}
	return bRet;
}

bool sha1_file_string(const char *filepath, std::string &strHash)
{
	unsigned char hashbuf[SHA1_SIZE] = {0};
	strHash.clear();
	if (sha1_file_byte(filepath, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, SHA1_SIZE);
	}
	return false;
}

bool sha1_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen)
{
    bool bRet = false;
    unsigned char *pFileData = NULL;
    unsigned int nFileSize = 0;
    struct _stat64i32 fs = {0};
    int fd = -1;
    if (NULL == filepath || NULL == hashbuf || 0 == hashlen || SHA1_SIZE > hashlen)
    {
        return false;
    }

    do 
    {
        if (0 != _wstat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
        {
            break;
        }
        fd = _wopen(filepath, O_RDONLY|O_BINARY, NULL);
        if (fd < 0)
        {
            break;
        }

        nFileSize = fs.st_size;
        pFileData = new unsigned char[nFileSize];
        if (NULL == pFileData)
        {
            break;
        }
        if (nFileSize != read(fd, pFileData, nFileSize))
        {
            break;
        }
        bRet = sha1_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
    } while (0);

    if (fd > 0)
    {
        close(fd);
    }
    if (pFileData)
    {
        delete[] pFileData;
        pFileData = NULL;
    }
    return bRet;
}

bool sha1_file_string(const wchar_t *filepath, std::string &strHash)
{
    unsigned char hashbuf[SHA1_SIZE] = {0};
    strHash.clear();
    if (sha1_file_byte(filepath, hashbuf, _countof(hashbuf)))
    {
        return HexToString(strHash, hashbuf, SHA1_SIZE);
    }
    return false;
}

// sha256
bool sha256_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen)
{
	if (NULL == buf || 0 == len || MAX_HASH_BUFFER_SIZE < len || 
		NULL == hashbuf || 0 == hashlen || SHA256_SIZE > hashlen)
	{
		return false;
	}
	memset(hashbuf, 0, hashlen);
	SHA256(buf, len, hashbuf);
	return true;
}

bool sha256_buffer_string(const unsigned char *buf, size_t len, std::string &strHash)
{
	unsigned char hashbuf[SHA256_SIZE] = {0};
	strHash.clear();
	if (sha256_buffer_byte(buf, len, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, SHA256_SIZE);
	}
	return false;
}

bool sha256_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen)
{
	bool bRet = false;
	unsigned char *pFileData = NULL;
	unsigned int nFileSize = 0;
	struct stat fs = {0};
	int fd = -1;
	if (NULL == filepath || NULL == hashbuf || 0 == hashlen || SHA256_SIZE > hashlen)
	{
		return false;
	}

	do 
	{
		if (0 != stat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
		{
			break;
		}
		fd = open(filepath, O_RDONLY|O_BINARY, NULL);
		if (fd < 0)
		{
			break;
		}

		nFileSize = fs.st_size;
		pFileData = new unsigned char[nFileSize];
		if (NULL == pFileData)
		{
			break;
		}
		if (nFileSize != read(fd, pFileData, nFileSize))
		{
			break;
		}
		bRet = sha256_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
	} while (0);

	if (fd > 0)
	{
		close(fd);
	}
	if (pFileData)
	{
		delete[] pFileData;
		pFileData = NULL;
	}
	return bRet;
}

bool sha256_file_string(const char *filepath, std::string &strHash)
{
	unsigned char hashbuf[SHA256_SIZE] = {0};
	strHash.clear();
	if (sha256_file_byte(filepath, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, SHA256_SIZE);
	}
	return false;
}
bool sha256_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen)
{
    bool bRet = false;
    unsigned char *pFileData = NULL;
    unsigned int nFileSize = 0;
    struct _stat64i32 fs = {0};
    int fd = -1;
    if (NULL == filepath || NULL == hashbuf || 0 == hashlen || SHA256_SIZE > hashlen)
    {
        return false;
    }

    do 
    {
        if (0 != _wstat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
        {
            break;
        }
        fd = _wopen(filepath, O_RDONLY|O_BINARY, NULL);
        if (fd < 0)
        {
            break;
        }

        nFileSize = fs.st_size;
        pFileData = new unsigned char[nFileSize];
        if (NULL == pFileData)
        {
            break;
        }
        if (nFileSize != read(fd, pFileData, nFileSize))
        {
            break;
        }
        bRet = sha256_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
    } while (0);

    if (fd > 0)
    {
        close(fd);
    }
    if (pFileData)
    {
        delete[] pFileData;
        pFileData = NULL;
    }
    return bRet;
}

bool sha256_file_string(const wchar_t *filepath, std::string &strHash)
{
    unsigned char hashbuf[SHA256_SIZE] = {0};
    strHash.clear();
    if (sha256_file_byte(filepath, hashbuf, _countof(hashbuf)))
    {
        return HexToString(strHash, hashbuf, SHA256_SIZE);
    }
    return false;
}

// sha384
bool sha384_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen)
{
	if (NULL == buf || 0 == len || MAX_HASH_BUFFER_SIZE < len || 
		NULL == hashbuf || 0 == hashlen || SHA384_SIZE > hashlen)
	{
		return false;
	}
	memset(hashbuf, 0, hashlen);
	SHA384(buf, len, hashbuf);
	return true;
}

bool sha384_buffer_string(const unsigned char *buf, size_t len, std::string &strHash)
{
	unsigned char hashbuf[SHA384_SIZE] = {0};
	strHash.clear();
	if (sha384_buffer_byte(buf, len, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, SHA384_SIZE);
	}
	return false;
}

bool sha384_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen)
{
	bool bRet = false;
	unsigned char *pFileData = NULL;
	unsigned int nFileSize = 0;
	struct stat fs = {0};
	int fd = -1;
	if (NULL == filepath || NULL == hashbuf || 0 == hashlen || SHA384_SIZE > hashlen)
	{
		return false;
	}

	do 
	{
		if (0 != stat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
		{
			break;
		}
		fd = open(filepath, O_RDONLY|O_BINARY, NULL);
		if (fd < 0)
		{
			break;
		}

		nFileSize = fs.st_size;
		pFileData = new unsigned char[nFileSize];
		if (NULL == pFileData)
		{
			break;
		}
		if (nFileSize != read(fd, pFileData, nFileSize))
		{
			break;
		}
		bRet = sha384_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
	} while (0);

	if (fd > 0)
	{
		close(fd);
	}
	if (pFileData)
	{
		delete[] pFileData;
		pFileData = NULL;
	}
	return bRet;
}

bool sha384_file_string(const char *filepath, std::string &strHash)
{
	unsigned char hashbuf[SHA384_SIZE] = {0};
	strHash.clear();
	if (sha384_file_byte(filepath, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, SHA384_SIZE);
	}
	return false;
}

bool sha384_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen)
{
    bool bRet = false;
    unsigned char *pFileData = NULL;
    unsigned int nFileSize = 0;
    struct _stat64i32 fs = {0};
    int fd = -1;
    if (NULL == filepath || NULL == hashbuf || 0 == hashlen || SHA384_SIZE > hashlen)
    {
        return false;
    }

    do 
    {
        if (0 != _wstat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
        {
            break;
        }
        fd = _wopen(filepath, O_RDONLY|O_BINARY, NULL);
        if (fd < 0)
        {
            break;
        }

        nFileSize = fs.st_size;
        pFileData = new unsigned char[nFileSize];
        if (NULL == pFileData)
        {
            break;
        }
        if (nFileSize != read(fd, pFileData, nFileSize))
        {
            break;
        }
        bRet = sha384_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
    } while (0);

    if (fd > 0)
    {
        close(fd);
    }
    if (pFileData)
    {
        delete[] pFileData;
        pFileData = NULL;
    }
    return bRet;
}

bool sha384_file_string(const wchar_t *filepath, std::string &strHash)
{
    unsigned char hashbuf[SHA384_SIZE] = {0};
    strHash.clear();
    if (sha384_file_byte(filepath, hashbuf, _countof(hashbuf)))
    {
        return HexToString(strHash, hashbuf, SHA384_SIZE);
    }
    return false;
}

// sha512
bool sha512_buffer_byte(const unsigned char *buf, size_t len, unsigned char *hashbuf, size_t hashlen)
{
	if (NULL == buf || 0 == len || MAX_HASH_BUFFER_SIZE < len || 
		NULL == hashbuf || 0 == hashlen || SHA512_SIZE > hashlen)
	{
		return false;
	}
	memset(hashbuf, 0, hashlen);
	SHA512(buf, len, hashbuf);
	return true;
}

bool sha512_buffer_string(const unsigned char *buf, size_t len, std::string &strHash)
{
	unsigned char hashbuf[SHA512_SIZE] = {0};
	strHash.clear();
	if (sha512_buffer_byte(buf, len, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, SHA512_SIZE);
	}
	return false;
}

bool sha512_file_byte(const char *filepath, unsigned char *hashbuf, size_t hashlen)
{
	bool bRet = false;
	unsigned char *pFileData = NULL;
	unsigned int nFileSize = 0;
	struct stat fs = {0};
	int fd = -1;
	if (NULL == filepath || NULL == hashbuf || 0 == hashlen || SHA512_SIZE > hashlen)
	{
		return false;
	}

	do 
	{
		if (0 != stat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
		{
			break;
		}
		fd = open(filepath, O_RDONLY|O_BINARY, NULL);
		if (fd < 0)
		{
			break;
		}

		nFileSize = fs.st_size;
		pFileData = new unsigned char[nFileSize];
		if (NULL == pFileData)
		{
			break;
		}
		if (nFileSize != read(fd, pFileData, nFileSize))
		{
			break;
		}
		bRet = sha512_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
	} while (0);

	if (fd > 0)
	{
		close(fd);
	}
	if (pFileData)
	{
		delete[] pFileData;
		pFileData = NULL;
	}
	return bRet;
}

bool sha512_file_string(const char *filepath, std::string &strHash)
{
	unsigned char hashbuf[SHA512_SIZE] = {0};
	strHash.clear();
	if (sha512_file_byte(filepath, hashbuf, _countof(hashbuf)))
	{
		return HexToString(strHash, hashbuf, SHA512_SIZE);
	}
	return false;
}


bool sha512_file_byte(const wchar_t *filepath, unsigned char *hashbuf, size_t hashlen)
{
    bool bRet = false;
    unsigned char *pFileData = NULL;
    unsigned int nFileSize = 0;
    struct _stat64i32 fs = {0};
    int fd = -1;
    if (NULL == filepath || NULL == hashbuf || 0 == hashlen || SHA512_SIZE > hashlen)
    {
        return false;
    }

    do 
    {
        if (0 != _wstat(filepath, &fs) || !(fs.st_mode & S_IFREG) || fs.st_size == 0 || fs.st_size > MAX_HASH_BUFFER_SIZE)
        {
            break;
        }
        fd = _wopen(filepath, O_RDONLY|O_BINARY, NULL);
        if (fd < 0)
        {
            break;
        }

        nFileSize = fs.st_size;
        pFileData = new unsigned char[nFileSize];
        if (NULL == pFileData)
        {
            break;
        }
        if (nFileSize != read(fd, pFileData, nFileSize))
        {
            break;
        }
        bRet = sha512_buffer_byte(pFileData, nFileSize, hashbuf, hashlen);
    } while (0);

    if (fd > 0)
    {
        close(fd);
    }
    if (pFileData)
    {
        delete[] pFileData;
        pFileData = NULL;
    }
    return bRet;
}

bool sha512_file_string(const wchar_t *filepath, std::string &strHash)
{
    unsigned char hashbuf[SHA512_SIZE] = {0};
    strHash.clear();
    if (sha512_file_byte(filepath, hashbuf, _countof(hashbuf)))
    {
        return HexToString(strHash, hashbuf, SHA512_SIZE);
    }
    return false;
}


#define MAX_HASH_LEN  512

bool com_ss_hash(E_HASH_TYPE hashType, const unsigned char* psz1Buf, unsigned int u1len, 
				 const unsigned char* psz2Buf, unsigned int u2len)
{
	bool bRet = false;
	std::string strHash1, strHash2;

	switch(hashType)
	{
	case md4:
		if(md4_buffer_string(psz1Buf, u1len, strHash1)
			&& md4_buffer_string(psz2Buf, u2len, strHash2))
		{
			bRet = true;
		}
		break;
	case md5:
		if(md5_buffer_string(psz1Buf, u1len, strHash1)
			&& md5_buffer_string(psz2Buf, u2len, strHash2))
		{
			bRet = true;
		}
		break;
	case sha1:
		if(sha1_buffer_string(psz1Buf, u1len, strHash1)
			&& sha1_buffer_string(psz2Buf, u2len, strHash2))
		{
			bRet = true;
		}
		break;
	case sha256:
		if(sha256_buffer_string(psz1Buf, u1len, strHash1)
			&& sha256_buffer_string(psz2Buf, u2len, strHash2))
		{
			bRet = true;
		}
		break;
	case sha384:
		if(sha384_buffer_string(psz1Buf, u1len, strHash1)
			&& sha384_buffer_string(psz2Buf, u2len, strHash2))
		{
			bRet = true;
		}
		break;
	case sha512:
		if(sha512_buffer_string(psz1Buf, u1len, strHash1)
			&& sha512_buffer_string(psz2Buf, u2len, strHash2))
		{
			bRet = true;
		}
		break;
	default:
		break;
	}

	if(bRet)
	{
		if(0 == _stricmp(strHash1.c_str(), strHash2.c_str()))
		{
			return true;
		}
	}

	return false;
}

bool com_sh_hash( E_HASH_TYPE hashType, const unsigned char* pszBuf, 
				 unsigned int ulen, const unsigned char* pszHash,
				 unsigned int uHashlen )
{
	bool bRet = false;
	std::string strHash;

	switch(hashType)
	{
	case md4:
		if(md4_buffer_string(pszBuf, ulen, strHash))
		{
			bRet = true;
		}
		break;
	case md5:
		if(md5_buffer_string(pszBuf, ulen, strHash))
		{
			bRet = true;
		}
		break;
	case sha1:
		if(sha1_buffer_string(pszBuf, ulen, strHash))
		{
			bRet = true;
		}
		break;
	case sha256:
		if(sha256_buffer_string(pszBuf, ulen, strHash))
		{
			bRet = true;
		}
		break;
	case sha384:
		if(sha384_buffer_string(pszBuf, ulen, strHash))
		{
			bRet = true;
		}
		break;
	case sha512:
		if(sha512_buffer_string(pszBuf, ulen, strHash))
		{
			bRet = true;
		}
		break;
	default:
		break;
	}

	if(bRet)
	{
		if(0 == _stricmp(strHash.c_str(), (char*)pszHash))
		{
			return true;
		}
	}

	return false;
}

bool com_ff_hash(E_HASH_TYPE hashType, char* p1file, char* p2file)
{
	bool bRet = false;
	// 	unsigned char sz1Buf[MAX_HASH_LEN] = {0};
	// 	unsigned char sz2Buf[MAX_HASH_LEN] = {0};
	std::string strHash1, strHash2;

	switch(hashType)
	{
	case md4:
		if(md4_file_string(p1file, strHash1)
			&& md4_file_string(p2file, strHash2))
		{
			bRet = true;
		}
		break;
	case md5:
		if(md5_file_string(p1file, strHash1)
			&& md5_file_string(p2file, strHash2))
		{
			bRet = true;
		}
		break;
	case sha1:
		if(sha1_file_string(p1file, strHash1)
			&& sha1_file_string(p2file, strHash2))
		{
			bRet = true;
		}
		break;
	case sha256:
		if(sha256_file_string(p1file, strHash1)
			&& sha256_file_string(p2file, strHash2))
		{
			bRet = true;
		}
		break;
	case sha384:
		if(sha384_file_string(p1file, strHash1)
			&& sha384_file_string(p2file, strHash2))
		{
			bRet = true;
		}
		break;
	case sha512:
		if(sha512_file_string(p1file, strHash1)
			&& sha512_file_string(p2file, strHash2))
		{
			bRet = true;
		}
		break;
	default:
		break;
	}

	if(bRet)
	{
		if(0 == _stricmp(strHash1.c_str(), strHash2.c_str()))
		{
			return true;
		}
	}

	return false;
}

bool com_fh_hash( E_HASH_TYPE hashType, char* p1file, 
				 const unsigned char* pszHash, unsigned int uHashlen )
{
	bool bRet = false;
	std::string strHash = "";

	switch(hashType)
	{
	case md4:
		if(md4_file_string(p1file, strHash))
		{
			bRet = true;
		}
		break;
	case md5:
		if(md5_file_string(p1file, strHash))
		{
			bRet = true;
		}
		break;
	case sha1:
		if(sha1_file_string(p1file, strHash))
		{
			bRet = true;
		}
		break;
	case sha256:
		if(sha256_file_string(p1file, strHash))
		{
			bRet = true;
		}
		break;
	case sha384:
		if(sha384_file_string(p1file, strHash))
		{
			bRet = true;
		}
		break;
	case sha512:
		if(sha512_file_string(p1file, strHash))
		{
			bRet = true;
		}
		break;
	default:
		break;
	}

	if(bRet)
	{
		if(0 == _stricmp(strHash.c_str(), (char*)pszHash))
		{
			return true;
		}
	}

	return false;
}
