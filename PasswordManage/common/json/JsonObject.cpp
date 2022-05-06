#include <string.h>
#include "JsonObject.h"
#include <assert.h>

#pragma warning (disable:4267)
CJson::CJson()
{

}

CJson::~CJson()
{
	m_JsonCmdRValue.clear();
	m_JsonCmdWValue.clear();
	m_JsonExtraValue.clear();
}

CJson& CJson::operator =(CJson& other)
{
	if(this == &other)
		return *this;

	m_JsonCmdRValue = other.m_JsonCmdRValue;
	m_JsonCmdWValue = other.m_JsonCmdWValue;
	m_JsonExtraValue = other.m_JsonExtraValue;
	return *this;
}

bool CJson::Parse(const char* pBuf)
{
	if (NULL == pBuf)
	{
		return false;
	}

	if (!m_JsonCmdRValue.empty())
	{
		m_JsonCmdRValue.clear();
	}
	return m_JsonReader.parse(pBuf, m_JsonCmdRValue);
}

bool CJson::IsCorrectValue()
{
	bool bRet = false;
	if (m_JsonCmdRValue.type() == Json::objectValue)
	{
		bRet = true;
	}
	return bRet;
}

std::string  CJson::GetStringValue(char* pKey)
{
	std::string strValue;
	if (NULL == pKey)
	{
		return strValue;
	}

	if (!m_JsonCmdRValue[pKey].empty() && m_JsonCmdRValue[pKey].isString())
	{
		strValue = m_JsonCmdRValue[pKey].asString();
	}
	return strValue;
}

Json::Value CJson::GetObjectValue(char* pKey)
{
	Json::Value ObjectValue;
	if (NULL == pKey)
	{
		return ObjectValue;
	}

	ObjectValue = m_JsonCmdRValue[pKey];
	return ObjectValue;
}

Json::Value CJson::GetArrayValue(char *pKey)
{
	Json::Value ArrayValue;
	if (NULL == pKey)
	{
		return ArrayValue;
	}

	ArrayValue = m_JsonCmdRValue[pKey];
	return ArrayValue;
}


int CJson::GetIntValue(char* pKey)
{
	int nRet = -1;
	if (NULL == pKey)
	{
		return nRet;
	}

	if (!m_JsonCmdRValue[pKey].empty() && m_JsonCmdRValue[pKey].isInt())
	{
		nRet = m_JsonCmdRValue[pKey].asInt();
	}
	return nRet;
}

unsigned int CJson::GetUintValue(char *pKey)
{
	unsigned int nRet = -1;
	if (pKey)
	{
		if (!m_JsonCmdRValue[pKey].empty() /*&& m_JsonCmdRValue[pKey].isUInt()*/)
		{
			nRet = m_JsonCmdRValue[pKey].asUInt();
		}
	}

	return nRet;
}

unsigned int CJson::GetArraySize(char* pKey)
{
	unsigned int nRet = 0;
	if (NULL == pKey)
	{
		return nRet;
	}

	if (!m_JsonCmdRValue[pKey].empty() && m_JsonCmdRValue[pKey].isArray())
	{
		nRet = m_JsonCmdRValue[pKey].size();
	}
	return nRet;
}

int CJson::GetArrayValue(char* pKey, int nIndex)
{
	int nRet = 0;
	if (NULL == pKey || nIndex < 0)
	{
		return nRet;
	}

	if (!m_JsonCmdRValue[pKey].empty() && m_JsonCmdRValue[pKey].isArray() && m_JsonCmdRValue[pKey][nIndex].isInt())
	{
		nRet = m_JsonCmdRValue[pKey][nIndex].asInt();
	}
	return nRet;	
}


void CJson::GetArrayValue(char* pKey, vector<int>& vecOut)
{
	if (NULL == pKey)
	{
		return;
	}

	int nSize = 0;
	int nValue = 0;
	Json::Value JsonValue = m_JsonCmdRValue[pKey];

	if (!JsonValue.empty() && JsonValue.isArray())
	{
		nSize = JsonValue.size();
		if (nSize == 0 && JsonValue[nSize].isInt())
		{
			nValue = JsonValue[nSize].asInt();
			vecOut.push_back(nValue);
		}
		else
		{
			for (int i = 0; i < nSize; i++)
			{
				if (JsonValue[i].isInt())
				{
					nValue = JsonValue[i].asInt();
					vecOut.push_back(nValue);
				}
			}
		}
	}
}

void CJson::GetArrayValue(char* pKey, vector<string>& vecOut)
{
	if (NULL == pKey)
	{
		return;
	}

	int nSize = 0;
	string strValue;
	Json::Value JsonValue = m_JsonCmdRValue[pKey];

	if (!JsonValue.empty() && JsonValue.isArray())
	{
		nSize = JsonValue.size();
		for (int i = 0; i < nSize; i++)
		{
			if (JsonValue[i].isString())
			{
				strValue = JsonValue[i].asString();
				vecOut.push_back(strValue);
			}
		}
	}
}

unsigned int CJson::GetUintValue(Json::Value &jValue, char *pKey)
{
	unsigned int nRet = -1;
	if (pKey)
	{
		if (!jValue[pKey].empty() /*&& jValue[pKey].isUInt()*/)
		{
			nRet = jValue[pKey].asUInt();
		}
	}

	return nRet;
}
int CJson::GetIntValue(Json::Value &jValue, char *pKey)
{
	unsigned int nRet = -1;
	if (pKey)
	{
		if (!jValue[pKey].empty() && jValue[pKey].isInt())
		{
			nRet = jValue[pKey].asInt();
		}
	}

	return nRet;
}
std::string CJson::GetStringValue(Json::Value &jValue, char* pKey)
{
	std::string strValue;
	if (NULL == pKey)
	{
		return strValue;
	}

	if (!jValue[pKey].empty() && jValue[pKey].isString())
	{
		strValue = jValue[pKey].asString();
	}
	return strValue;
}

Json::Value CJson::GetArrayValue(Json::Value &jValue, char *pKey)
{
    Json::Value ArrayValue;
    if (NULL == pKey)
    {
        return ArrayValue;
    }

    if (!jValue[pKey].empty())
    {
        ArrayValue = jValue[pKey];
    }
    return ArrayValue;
}


void CJson::RemoveParseValue(const char* pKey)
{
	if (NULL != pKey)
	{
		m_JsonCmdRValue.removeMember(pKey);
	}
}

void CJson::InitWriteValue(Json::Value& initValue)
{
	m_JsonCmdWValue = initValue;
}

void CJson::AddValue(char* pKey, const char* pValue)
{
	if (NULL == pKey || NULL == pValue)
	{
		return;
	}

	m_JsonCmdWValue[pKey] = pValue;
}

void CJson::AddValue(char* pKey, int nValue)
{
	if (NULL == pKey)
	{
		return;
	}

	m_JsonCmdWValue[pKey] = nValue;
}

void CJson::AddValue(char* pKey, unsigned int nValue)
{
	if (NULL == pKey)
	{
		return;
	}

	m_JsonCmdWValue[pKey] = nValue;
}

void CJson::AddArrayValue(char* pKey, vector<int>& vecValue)
{
	if (NULL == pKey || vecValue.size() == 0)
	{
		return;
	}

	Json::Value arrayValue;
	for (unsigned int i = 0; i < vecValue.size(); i++)
	{
		arrayValue.append(vecValue[i]);
	}
	m_JsonCmdWValue[pKey] = arrayValue;
}

void CJson::AppendJson(const CJson *value)
{	
	assert(NULL!=value);
	CJson jsTemp;
	if(value)
	{
		jsTemp.m_JsonCmdWValue = ((CJson*)value)->m_JsonCmdWValue;
	}

	m_JsonCmdWValue.append(jsTemp.m_JsonCmdWValue);
}

CJson CJson::GetArrayItem(unsigned int nIndex)
{
	CJson JsTemp;
	JsTemp.m_JsonCmdRValue=m_JsonCmdRValue[nIndex];

	return JsTemp;
}

unsigned int CJson::GetArraySize()
{
	return m_JsonCmdRValue.size();
}

void CJson::AddArrayValue(char* pKey, vector<unsigned __int64>& vecValue)
{
	char szValue[260];
	Json::Value arrayValue;
	if (NULL == pKey || vecValue.size() == 0)
	{
		return;
	}

	for (unsigned int i = 0; i < vecValue.size(); i++)
	{
		memset(szValue, 0, sizeof(szValue));
		_ui64toa_s(vecValue[i], szValue, _countof(szValue), 10);
		arrayValue.append(szValue);
	}
	m_JsonCmdWValue[pKey] = arrayValue;
}

void CJson::AddArrayValue(char* pKey, const vector<std::string>& vecValue)
{
	Json::Value arrayValue;
	if (NULL == pKey || vecValue.size() == 0)
	{
		return;
	}

	for (unsigned int i = 0; i < vecValue.size(); i++)
	{
		arrayValue.append(vecValue[i].c_str());
	}
	m_JsonCmdWValue[pKey] = arrayValue;
}

void CJson::AddValue(char* pKey, Json::Value ObjectValue)
{
	if (NULL == pKey)
	{
		return;
	}

	m_JsonCmdWValue[pKey] = ObjectValue;
}

void CJson::SetValue(Json::Value &jValue, const char* pKey, unsigned __int64 nValue)
{
	char szValue[260] = {0};
	if (NULL == pKey)
	{
		return;
	}
	_ui64toa_s(nValue, szValue, _countof(szValue), 10);
	jValue[pKey] = szValue;
}

void CJson::SetValue(Json::Value &jValue, const char* pKey, int nValue)
{
	if (NULL == pKey)
	{
		return;
	}
	jValue[pKey] = nValue;
}
void CJson::SetValue(Json::Value &jValue, const char* pKey, unsigned int nValue)
{
	if (NULL == pKey)
	{
		return;
	}
	jValue[pKey] = nValue;
}

void CJson::SetValue(Json::Value &jValue, const char* pKey, const char* pValue)
{
	if (NULL == pKey || NULL == pValue)
	{
		return;
	}
	jValue[pKey] = pValue;
}

void CJson::SetArrayValue(Json::Value &jValue, const char* pKey, Json::Value &ObjectValue)
{
    if (NULL == pKey)
    {
        return;
    }

    jValue[pKey] = ObjectValue;
}

void CJson::SetExtraValue(Json::Value ExtraValue)
{
	m_JsonExtraValue.clear();
	if (ExtraValue.type() != Json::nullValue)
	{
		m_JsonExtraValue = ExtraValue;
	}
}

void CJson::FastWrite(std::string &strBuf)
{
	strBuf = m_JsonFastWriter.write(m_JsonCmdWValue);
}

std::string CJson::FastWrite()
{
	return m_JsonFastWriter.write(m_JsonCmdWValue);
}

void CJson::ClearWriteJson()
{
	if (!m_JsonCmdWValue.empty())
	{
		m_JsonCmdWValue.clear();
	}
}

