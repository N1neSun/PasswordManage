
#ifndef _JSON_OBJECT_H_
#define _JSON_OBJECT_H_

#include <xstring>
#include <json/json.h>
#include <vector>
#include <string>
#include <set>



using namespace std;

class CJson
{
public:
	CJson();
	virtual ~CJson();
	CJson& operator =(CJson& other);


	bool Parse(const char* pBuf);
	bool IsCorrectValue();
	std::string GetStringValue(char* pKey);
	int  GetIntValue(char* pKey);
	unsigned int GetUintValue(char *pKey);
	unsigned int GetArraySize(char* pKey);
	int  GetArrayValue(char* pKey, int nIndex);
	void GetArrayValue(char* pKey, vector<int>& vecOut);
	void GetArrayValue(char* pKey, vector<string>& vecOut);
	void RemoveParseValue(const char* pKey);
	Json::Value GetObjectValue(char* pKey);
    Json::Value GetArrayValue(char *pKey);
	Json::Value& GetParseValue() { return m_JsonCmdRValue;}
	unsigned int GetUintValue(Json::Value &jValue, char *pKey);
	int GetIntValue(Json::Value &jValue, char *pKey);
	std::string GetStringValue(Json::Value &jValue, char* pKey);
    Json::Value GetArrayValue(Json::Value &jValue, char *pKey);


	void InitWriteValue(Json::Value& initValue);
	void SetExtraValue(Json::Value ExtraValue);

	void AddValue(char* pKey, const char* pValue);
	void AddValue(char* pKey, int nValue);
	void AddValue(char* pKey, unsigned int nValue);
	void AddValue(char* pKey, Json::Value ObjectValue);
	void AddArrayValue(char*pKey, vector<int>& vecValue);
	void AddArrayValue(char* pKey, vector<unsigned __int64>& vecValue);
	void AddArrayValue(char* pKey, const vector<std::string>& vecValue);
	void AppendJson(const CJson* value);
	CJson GetArrayItem(unsigned int nIndex);
	unsigned int GetArraySize();


	void SetValue(Json::Value &jValue, const char* pKey, unsigned __int64 nValue);
	void SetValue(Json::Value &jValue, const char* pKey, int nValue);
	void SetValue(Json::Value &jValue, const char* pKey, unsigned int nValue);
    void SetValue(Json::Value &jValue, const char* pKey, const char* pValue);
    void SetArrayValue(Json::Value &jValue, const char* pKey, Json::Value &ObjectValue);

	void FastWrite(std::string &strBuf);
	std::string FastWrite();
	void ClearWriteJson();

	Json::Value m_JsonCmdRValue;
	Json::Value m_JsonCmdWValue;
	Json::Value m_JsonExtraValue;

	Json::Reader m_JsonReader;
	Json::FastWriter m_JsonFastWriter;

};

#endif

