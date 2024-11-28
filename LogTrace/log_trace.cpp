/*
 * Author: Praveen Mandula
 * Date: 11/15/2024
 * Description : Logger class is used for logging debug messages into Console or File based on logConfiguration
 */
#include "log_trace.h"

Logger& Logger::getInstance() {
    static Logger instance; // Guaranteed to be lazy-initialized and thread-safe in C++11 and later
    return instance;
}

Logger::Logger() {
    readLogConfig("Config/logConfig.json");
}

Logger::~Logger() {
    if (fileStream && fileStream->is_open()) {
        fileStream->close();
    }
}

void Logger::readLogConfig(const char* configFilePath) {
    // ReadLogConfiguration
    
    m_logConfigReader.readConfiguration(configFilePath);

    logLevelThreshold       =   stringToLogLevel(m_logConfigReader.getKeyValue("logLevel"));
    consoleEnabled          =   (m_logConfigReader.getKeyValue("enableConsoleLogging") == "true") ? true : false;
    fileEnabled             =   (m_logConfigReader.getKeyValue("enableFileLogging") == "true") ? true : false;
    logFilePath             =   m_logConfigReader.getKeyValue("logFilePath");

    if (fileEnabled) {
        fileStream = std::make_unique<std::ofstream>(logFilePath, std::ios::app);
        if (!fileStream->is_open()) {
            LogMessage(LogLevel::ERROR_R, "Failed to open config log file : ", configFilePath);
        }
    }
}

LogLevel Logger::stringToLogLevel(const std::string& level) const {
    if (level == "DEBUG") return LogLevel::DEBUG;
    if (level == "INFO") return LogLevel::INFO;
    if (level == "WARNING") return LogLevel::WARNING;
    if (level == "ERROR") return LogLevel::ERROR_R;
    if (level == "CRITICAL") return LogLevel::CRITICAL;
    return LogLevel::INFO;
}

std::string Logger::toString(LogLevel level) const {
    switch (level) {
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR_R: return "ERROR";
    case LogLevel::CRITICAL: return "CRITICAL";
    default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level >= logLevelThreshold) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::string logMessage = "[" + toString(level) + "] " + message;

        if (consoleEnabled) {
            std::cout << logMessage << std::endl;
        }
        if (fileEnabled && fileStream && fileStream->is_open()) {
            (*fileStream) << logMessage << std::endl;
        }
    }
}

void Logger::log(LogLevel level, const std::ostringstream& messageStream) {
    log(level, messageStream.str()); // Convert to string and call the original log method
}

// Helper Macro for logging message
#define LogMessage(level, ...) Logger::getInstance().log(level, __VA_ARGS__)