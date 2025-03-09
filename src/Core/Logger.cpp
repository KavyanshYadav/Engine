#include "Logger.h"

void Logger::SetLogToFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile.close();
    }
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        // Use a direct cout here to avoid recursive logging when file fails to open
        std::cout << "CRITICAL: Failed to open log file: " << filename << std::endl;
    }
} 