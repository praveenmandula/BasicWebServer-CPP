#pragma once
#include "cpp_standard_includes.h"
#include "third_party/nlohmann/json.hpp"

class LogConfigReader {
public:
	int readConfiguration(std::string& filePath);
	int readConfiguration(const char* filePath);

	std::string getKeyValue(std::string& key);
	std::string getKeyValue(const char* key);

private:
	int parseJsonConfig(const std::string& filePath);
	int parseXmlConfig(const std::string& filePath);
	int parseKeyValueConfig(const std::string& filePath);

	typedef std::map<std::string, std::string> logConfigData;
	logConfigData m_logConfigData;
};