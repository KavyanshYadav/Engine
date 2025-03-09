#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <sstream>
#include <memory>
#include <deque>
#include <chrono>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    CRITICAL
};

struct LogEntry {
    std::string message;
    LogLevel level;
    std::chrono::system_clock::time_point timestamp;
};

class Logger {
public:
    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    void SetLogLevel(LogLevel level) { currentLevel = level; }
    void SetLogToFile(const std::string& filename);
    void SetMaxLogEntries(size_t max) { maxLogEntries = max; }

    // Get stored logs
    const std::deque<LogEntry>& GetLogEntries() const { return logEntries; }
    void ClearLogs() { 
        std::lock_guard<std::mutex> lock(logMutex);
        logEntries.clear(); 
    }

    template<typename... Args>
    void Debug(Args... args) {
        Log(LogLevel::DEBUG, args...);
    }

    template<typename... Args>
    void Info(Args... args) {
        Log(LogLevel::INFO, args...);
    }

    template<typename... Args>
    void Warning(Args... args) {
        Log(LogLevel::WARNING, args...);
    }

    template<typename... Args>
    void Critical(Args... args) {
        Log(LogLevel::CRITICAL, args...);
    }

private:
    Logger() : currentLevel(LogLevel::INFO), maxLogEntries(1000) {}
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    template<typename T>
    void LogSingle(std::stringstream& ss, T&& arg) {
        ss << std::forward<T>(arg);
    }

    template<typename T, typename... Args>
    void LogSingle(std::stringstream& ss, T&& arg, Args&&... args) {
        ss << std::forward<T>(arg);
        LogSingle(ss, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Log(LogLevel level, Args&&... args) {
        if (level < currentLevel) return;

        std::stringstream ss;
        LogSingle(ss, std::forward<Args>(args)...);

        std::lock_guard<std::mutex> lock(logMutex);
        
        // Create log entry
        LogEntry entry{
            ss.str(),
            level,
            std::chrono::system_clock::now()
        };

        // Add to circular buffer
        logEntries.push_back(entry);
        if (logEntries.size() > maxLogEntries) {
            logEntries.pop_front();
        }

        // Format output with timestamp and level
        std::time_t now = std::chrono::system_clock::to_time_t(entry.timestamp);
        std::string timestamp = std::ctime(&now);
        timestamp.pop_back(); // Remove newline

        std::cout << "[" << timestamp << "] " << GetLevelString(level) << ": " << ss.str() << std::endl;
        
        if (logFile.is_open()) {
            logFile << "[" << timestamp << "] " << GetLevelString(level) << ": " << ss.str() << std::endl;
            logFile.flush();
        }
    }

    const char* GetLevelString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }

    LogLevel currentLevel;
    std::mutex logMutex;
    std::ofstream logFile;
    std::deque<LogEntry> logEntries;
    size_t maxLogEntries;
}; 