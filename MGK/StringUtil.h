#pragma once
#include "EngineHeader.h"
 
namespace StringUtil {
	void InputChar(std::string& Str, unsigned char AddChar, bool UseNewLine=false);
	void AddString(std::string& Str, std::string AddStr);
	void EraseString(std::string& Str);
	void RemoveString(std::string& Str, std::string RemoveStr);
	std::wstring Wstring(const std::string& Str);
}