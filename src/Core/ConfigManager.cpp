#include "Core/ConfigManager.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <glad/glad.h>

ConfigManager& ConfigManager::GetInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() 
    : systemRequirementsMet(false)
    , recommendedRequirementsMet(false) {
}

void ConfigManager::Initialize() {
    LoadConfig();
    ValidateSystemRequirements();
    SaveSystemInfo();
    SaveConfig();
}

void ConfigManager::SaveConfig() {
    std::string configPath = GetConfigFilePath();
    std::ofstream file(configPath);
    if (!file.is_open()) {
        Logger::GetInstance().Critical("Failed to open config file for writing: " + configPath);
        return;
    }

    // Write a header to the config file
    auto now = std::time(nullptr);
    auto localTime = std::localtime(&now);
    char timeStr[26];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);

    file << "# Engine Configuration File\n";
    file << "# Generated on: " << timeStr << "\n\n";

    // Write system info section
    file << "[System]\n";
    for (const auto& [key, value] : configData) {
        if (key.find("System.") == 0) {
            file << key << "=" << value << "\n";
        }
    }

    // Write other sections later
    file << "\n[Settings]\n";
    for (const auto& [key, value] : configData) {
        if (key.find("System.") != 0) {
            file << key << "=" << value << "\n";
        }
    }
    
    Logger::GetInstance().Info("Configuration saved to: " + configPath);
}

void ConfigManager::LoadConfig() {
    std::ifstream file(GetConfigFilePath());
    if (!file.is_open()) {
        Logger::GetInstance().Warning("No config file found, creating new one");
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t delimPos = line.find('=');
        if (delimPos != std::string::npos) {
            std::string key = line.substr(0, delimPos);
            std::string value = line.substr(delimPos + 1);
            configData[key] = value;
        }
    }
}

void ConfigManager::SaveSystemInfo() {
    auto& sysInfo = SystemInfo::GetInstance();
    
    // Save CPU info
    const auto& cpuInfo = sysInfo.GetCPUInfo();
    SetConfigValue("System.CPU.Vendor", cpuInfo.vendor);
    SetConfigValue("System.CPU.Brand", cpuInfo.brand);
    SetConfigValue("System.CPU.Cores", cpuInfo.numCores);
    SetConfigValue("System.CPU.Threads", cpuInfo.numThreads);
    SetConfigValue("System.CPU.Architecture", cpuInfo.architecture);
    
    std::stringstream features;
    for (const auto& feature : cpuInfo.features) {
        features << feature << ";";
    }
    SetConfigValue("System.CPU.Features", features.str());

    // Save GPU info
    const auto& gpuInfo = sysInfo.GetGPUInfo();
    SetConfigValue("System.GPU.Vendor", gpuInfo.vendor);
    SetConfigValue("System.GPU.Renderer", gpuInfo.renderer);
    SetConfigValue("System.GPU.Version", gpuInfo.version);
    SetConfigValue("System.GPU.GLSLVersion", gpuInfo.glslVersion);
    SetConfigValue("System.GPU.TotalMemory", std::to_string(gpuInfo.totalMemoryMB));

    std::stringstream extensions;
    for (const auto& ext : gpuInfo.extensions) {
        extensions << ext << ";";
    }
    SetConfigValue("System.GPU.Extensions", extensions.str());

    // Save memory info
    const auto& memInfo = sysInfo.GetMemoryInfo();
    SetConfigValue("System.Memory.TotalPhysical", std::to_string(memInfo.totalPhysicalMemoryMB));
    SetConfigValue("System.Memory.AvailablePhysical", std::to_string(memInfo.availablePhysicalMemoryMB));
    SetConfigValue("System.Memory.TotalVirtual", std::to_string(memInfo.totalVirtualMemoryMB));
    SetConfigValue("System.Memory.AvailableVirtual", std::to_string(memInfo.availableVirtualMemoryMB));

    // Save OS info
    SetConfigValue("System.OS", sysInfo.GetOSInfo());
}

void ConfigManager::ValidateSystemRequirements() {
    auto& sysInfo = SystemInfo::GetInstance();
    const auto& cpuInfo = sysInfo.GetCPUInfo();
    const auto& memInfo = sysInfo.GetMemoryInfo();
    
    // Check minimum requirements
    bool minCPUMet = cpuInfo.numCores >= MIN_CPU_CORES;
    bool minMemoryMet = memInfo.totalPhysicalMemoryMB >= MIN_MEMORY_MB;
    
    // Check OpenGL version
    int glMajor, glMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
    glGetIntegerv(GL_MINOR_VERSION, &glMinor);
    
    bool minOpenGLMet = (glMajor > MIN_OPENGL_MAJOR) || 
                        (glMajor == MIN_OPENGL_MAJOR && glMinor >= MIN_OPENGL_MINOR);
    
    systemRequirementsMet = minCPUMet && minMemoryMet && minOpenGLMet;
    
    // Check recommended requirements
    bool recCPUMet = cpuInfo.numCores >= REC_CPU_CORES;
    bool recMemoryMet = memInfo.totalPhysicalMemoryMB >= REC_MEMORY_MB;
    bool recOpenGLMet = (glMajor > REC_OPENGL_MAJOR) || 
                        (glMajor == REC_OPENGL_MAJOR && glMinor >= REC_OPENGL_MINOR);
    
    recommendedRequirementsMet = recCPUMet && recMemoryMet && recOpenGLMet;
    
    // Log system requirements status
    auto& logger = Logger::GetInstance();
    if (!systemRequirementsMet) {
        logger.Warning("System does not meet minimum requirements:");
        if (!minCPUMet) logger.Warning("- CPU cores below minimum requirement");
        if (!minMemoryMet) logger.Warning("- Memory below minimum requirement");
        if (!minOpenGLMet) logger.Warning("- OpenGL version below minimum requirement");
    } else if (!recommendedRequirementsMet) {
        logger.Info("System meets minimum but not recommended requirements");
    } else {
        logger.Info("System meets all recommended requirements");
    }
}

std::string ConfigManager::GetConfigFilePath() const {
    return "engine_config.ini";
}

bool ConfigManager::MeetsMinimumRequirements() const {
    return systemRequirementsMet;
}

bool ConfigManager::MeetsRecommendedRequirements() const {
    return recommendedRequirementsMet;
}

std::string ConfigManager::GetConfigValue(const std::string& key) const {
    auto it = configData.find(key);
    return (it != configData.end()) ? it->second : "";
}

int ConfigManager::GetConfigValueInt(const std::string& key) const {
    return std::stoi(GetConfigValue(key));
}

float ConfigManager::GetConfigValueFloat(const std::string& key) const {
    return std::stof(GetConfigValue(key));
}

bool ConfigManager::GetConfigValueBool(const std::string& key) const {
    std::string value = GetConfigValue(key);
    return value == "true" || value == "1";
}

void ConfigManager::SetConfigValue(const std::string& key, const std::string& value) {
    configData[key] = value;
}

void ConfigManager::SetConfigValue(const std::string& key, int value) {
    SetConfigValue(key, std::to_string(value));
}

void ConfigManager::SetConfigValue(const std::string& key, float value) {
    SetConfigValue(key, std::to_string(value));
}

void ConfigManager::SetConfigValue(const std::string& key, bool value) {
    SetConfigValue(key, value ? "true" : "false");
} 