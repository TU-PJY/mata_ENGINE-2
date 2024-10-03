#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "tinyxml/tinyxml.h"
#include <iostream>
#include <string>

class ScriptUtil {
private:
	TiXmlDocument Doc{};
	TiXmlElement* Root{};
	bool FileExist{};

public:
	void Import(const char* FileName);
	int LoadIntData(const char* CategoryName, const char* DataName);
	float LoadFloatData(const char* CategoryName, const char* DataName);
	const char* LoadStringData(const char* CategoryName, const char* DataName);
	void Release();

private:
	TiXmlElement* FindCategory(const char* CategoryName);
	const char* FindData(const char* CategoryName, const char* DataName);
	int GetIntData(TiXmlElement* CategoryVar, const char* DataName);
	float GetFloatData(TiXmlElement* CategoryVar, const char* DataName);
	const char* GetStringData(TiXmlElement* CategoryVar, const char* DataName);
};