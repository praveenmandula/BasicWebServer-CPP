/*
 * Author: Praveen Mandula
 * Date: 11/14/2024
 * Description : LogConfigReader class is used for reading configuration from .json,*.txt,*.ini or .xml file
 */
#include "log_config_reader.h"
#include "log_trace.h"

int LogConfigReader::readConfiguration(std::string& filePath)
{
    if ( m_logConfigData.empty() ) {

        if (filePath.ends_with(".json")) {
            return parseJsonConfig(filePath);
        }
        else if (filePath.ends_with(".xml")) {
            return parseXmlConfig(filePath);
        }
        else if (filePath.ends_with(".ini") || filePath.ends_with(".txt")) {
            return parseKeyValueConfig(filePath);
        }
        return 0;
    }
    return 1;
}

int LogConfigReader::readConfiguration(const char* filePath)
{
    std::string filePathStr(filePath);
    return readConfiguration(filePathStr);
}

std::string LogConfigReader::getKeyValue(std::string& key)
{
    auto it = m_logConfigData.find(key);
    if (it != m_logConfigData.end()) {
        return it->second;
    }
    return "";
}

std::string LogConfigReader::getKeyValue(const char* key)
{
    std::string keyStr(key);
    return getKeyValue(keyStr);
}

int LogConfigReader::parseJsonConfig(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        LogMessage(LogLevel::ERROR_R, "Could not open config file:", filePath);
        return -1;
    }
    nlohmann::json config;
    file >> config;

    // Iterate through the JSON object
    for (auto& [key, value] : config.items()) {
        // Convert all types to string
        if (value.is_string()) {
            m_logConfigData[key] = value.get<std::string>();
        }
        else if (value.is_number()) {
            m_logConfigData[key] = std::to_string(value.get<int>());
        }
        else if (value.is_boolean()) {
            m_logConfigData[key] = value.get<bool>() ? "true" : "false";
        }
        else if (value.is_null()) {
            m_logConfigData[key] = "null";  // Optionally handle null values
        }
        else {
            LogMessage(LogLevel::ERROR_R, "Configuration key not found");
        }
    }
    return 1;
}

int LogConfigReader::parseXmlConfig(const std::string& filePath)
{
    //tinyxml2::XMLDocument doc;
    //if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
    //    throw std::runtime_error("Could not open XML config file.");
    //}

    //tinyxml2::XMLElement* root = doc.FirstChildElement();
    //for (tinyxml2::XMLElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
    //    const char* key = element->Name();
    //    const char* value = element->GetText();
    //    if (key && value) {
    //        m_logConfigData[key] = value;
    //    }
    //}

    return 0;
}

int LogConfigReader::parseKeyValueConfig(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        LogMessage(LogLevel::ERROR_R, "Failed to open config log file : ", filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t delimiter_pos = line.find('=');
        if (delimiter_pos != std::string::npos) {
            std::string key = line.substr(0, delimiter_pos);
            std::string value = line.substr(delimiter_pos + 1);
            m_logConfigData[key] = value;
        }
    }
    return 0;
}