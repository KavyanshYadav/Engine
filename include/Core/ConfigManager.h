#pragma once
#include "Core/SystemInfo.h"
#include <string>
#include <memory>
#include <map>

class ConfigManager {
public:
    static ConfigManager& GetInstance();
    
    // Delete copy constructor and assignment operator
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    // Initialize configuration
    void Initialize();
    
    // Save and load configuration
    void SaveConfig();
    void LoadConfig();
    
    // Get system requirements status
    bool MeetsMinimumRequirements() const;
    bool MeetsRecommendedRequirements() const;
    
    // Get configuration values
    std::string GetConfigValue(const std::string& key) const;
    int GetConfigValueInt(const std::string& key) const;
    float GetConfigValueFloat(const std::string& key) const;
    bool GetConfigValueBool(const std::string& key) const;
    
    // Set configuration values
    void SetConfigValue(const std::string& key, const std::string& value);
    void SetConfigValue(const std::string& key, int value);
    void SetConfigValue(const std::string& key, float value);
    void SetConfigValue(const std::string& key, bool value);

private:
    ConfigManager();  // Private constructor for singleton
    
    void SaveSystemInfo();
    void ValidateSystemRequirements();
    std::string GetConfigFilePath() const;
    
    std::map<std::string, std::string> configData;
    bool systemRequirementsMet;
    bool recommendedRequirementsMet;
    
    // Minimum system requirements
    static constexpr int MIN_CPU_CORES = 2;
    static constexpr size_t MIN_MEMORY_MB = 2048;  // 2GB
    static constexpr int MIN_OPENGL_MAJOR = 4;
    static constexpr int MIN_OPENGL_MINOR = 5;
    
    // Recommended system requirements
    static constexpr int REC_CPU_CORES = 4;
    static constexpr size_t REC_MEMORY_MB = 8192;  // 8GB
    static constexpr int REC_OPENGL_MAJOR = 4;
    static constexpr int REC_OPENGL_MINOR = 6;
}; 