#pragma once
#include <string>
#include <vector>
#include <memory>

struct GPUInfo {
    std::string vendor;
    std::string renderer;
    std::string version;
    std::string glslVersion;
    int totalMemoryMB;
    std::vector<std::string> extensions;
};

struct CPUInfo {
    std::string vendor;
    std::string brand;
    int numCores;
    int numThreads;
    std::string architecture;
    int cacheSizeKB;
    std::vector<std::string> features;
};

struct SystemMemoryInfo {
    size_t totalPhysicalMemoryMB;
    size_t availablePhysicalMemoryMB;
    size_t totalVirtualMemoryMB;
    size_t availableVirtualMemoryMB;
};

class SystemInfo {
public:
    static SystemInfo& GetInstance();
    
    // Delete copy constructor and assignment operator
    SystemInfo(const SystemInfo&) = delete;
    SystemInfo& operator=(const SystemInfo&) = delete;

    // System information getters
    const GPUInfo& GetGPUInfo() const { return gpuInfo; }
    const CPUInfo& GetCPUInfo() const { return cpuInfo; }
    const SystemMemoryInfo& GetMemoryInfo() const { return memoryInfo; }
    const std::string& GetOSInfo() const { return osInfo; }
    
    // Refresh system information
    void RefreshSystemInfo();

private:
    SystemInfo();  // Private constructor for singleton
    
    void GatherGPUInfo();
    void GatherCPUInfo();
    void GatherMemoryInfo();
    void GatherOSInfo();

    GPUInfo gpuInfo;
    CPUInfo cpuInfo;
    SystemMemoryInfo memoryInfo;
    std::string osInfo;
}; 