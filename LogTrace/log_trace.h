#pragma once
#include "cpp_standard_includes.h"
#include "log_config_reader.h"

// Shared DLL support, Enable this and Change project type to DLL to generate DLL
// LOGGER_API - Add to the class

//#if defined(_WIN32) || defined(_WIN64)
//#ifdef LOGTRACE_EXPORTS
//#define LOGGER_API __declspec(dllexport)
//#else
//#define LOGGER_API __declspec(dllimport)
//#endif
//#else
//#define LOGGER_API
//#endif

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR_R, // getting compilation error if we declare ERROR
    CRITICAL
};

// Singleton Logger Class
class Logger {
public:
    static Logger& getInstance();   // Singleton instance accessor

    // logging methods
    void log(LogLevel level, const std::string& message);
    void log(LogLevel level, const std::ostringstream& messageStream);
    template<typename... Args>
    void log(LogLevel level, Args... args) {
        std::ostringstream oss;
        (oss << ... << args);   // Fold expression to handle multiple arguments
        log(level, oss.str());  // Call existing log function
    }

private:
    Logger();                                  // Private constructor
    Logger(const Logger&) = delete;            // Prevent copy-construction
    Logger& operator=(const Logger&) = delete; // Prevent assignment
    ~Logger();

    std::mutex logMutex;    // Mutex for thread-safety
    std::unique_ptr<std::ofstream> fileStream;

    // LogConfigurationDetails
    LogLevel logLevelThreshold;
    bool     consoleEnabled;
    bool     fileEnabled;
    std::string logFilePath;

    void readLogConfig(const char* configFilePath);
    LogLevel stringToLogLevel(const std::string& level) const;
    std::string toString(LogLevel level) const;

    LogConfigReader m_logConfigReader;  
};

// Helper Macro for logging message
#define LogMessage(level, ...) Logger::getInstance().log(level, __VA_ARGS__)
