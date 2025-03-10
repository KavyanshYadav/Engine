#include "Core/SystemInfo.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <intrin.h>
#include <algorithm>
#include <sstream>

SystemInfo& SystemInfo::GetInstance() {
    static SystemInfo instance;
    return instance;
}

SystemInfo::SystemInfo() {
    RefreshSystemInfo();
}

void SystemInfo::RefreshSystemInfo() {
    GatherGPUInfo();
    GatherCPUInfo();
    GatherMemoryInfo();
    GatherOSInfo();
}

void SystemInfo::GatherGPUInfo() {
    gpuInfo.vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    gpuInfo.renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    gpuInfo.version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    gpuInfo.glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Get GPU memory info (Note: GLAD doesn't support direct GPU memory query)
    gpuInfo.totalMemoryMB = 0;  // Set to 0 as we can't reliably get this info with GLAD

    // Get extensions
    GLint numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    for (GLint i = 0; i < numExtensions; i++) {
        const char* extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
        gpuInfo.extensions.push_back(extension);
    }
}

void SystemInfo::GatherCPUInfo() {
    int cpuInfoArray[4] = {-1};  // Renamed to avoid confusion
    char vendor[13];
    
    // Get vendor
    __cpuid(cpuInfoArray, 0);
    memcpy(vendor, &cpuInfoArray[1], 4);
    memcpy(vendor + 4, &cpuInfoArray[3], 4);
    memcpy(vendor + 8, &cpuInfoArray[2], 4);
    vendor[12] = '\0';
    this->cpuInfo.vendor = vendor;

    // Get brand
    char brand[49];
    __cpuid(cpuInfoArray, 0x80000002);
    memcpy(brand, cpuInfoArray, sizeof(cpuInfoArray));
    __cpuid(cpuInfoArray, 0x80000003);
    memcpy(brand + 16, cpuInfoArray, sizeof(cpuInfoArray));
    __cpuid(cpuInfoArray, 0x80000004);
    memcpy(brand + 32, cpuInfoArray, sizeof(cpuInfoArray));
    brand[48] = '\0';
    this->cpuInfo.brand = brand;

    // Get CPU features
    __cpuid(cpuInfoArray, 1);
    
    // Get number of cores and threads
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    this->cpuInfo.numCores = sysInfo.dwNumberOfProcessors;
    
    // Get logical processors (threads)
    DWORD_PTR processAffinityMask, systemAffinityMask;
    GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask);
    this->cpuInfo.numThreads = 0;
    for (DWORD_PTR mask = systemAffinityMask; mask; mask >>= 1) {
        if (mask & 1) {
            this->cpuInfo.numThreads++;
        }
    }

    // Get architecture
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            this->cpuInfo.architecture = "x64";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            this->cpuInfo.architecture = "x86";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            this->cpuInfo.architecture = "ARM";
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            this->cpuInfo.architecture = "ARM64";
            break;
        default:
            this->cpuInfo.architecture = "Unknown";
    }

    // Get CPU features
    if (cpuInfoArray[3] & (1 << 25)) this->cpuInfo.features.push_back("SSE");
    if (cpuInfoArray[3] & (1 << 26)) this->cpuInfo.features.push_back("SSE2");
    if (cpuInfoArray[2] & (1 << 0))  this->cpuInfo.features.push_back("SSE3");
    if (cpuInfoArray[2] & (1 << 19)) this->cpuInfo.features.push_back("SSE4.1");
    if (cpuInfoArray[2] & (1 << 20)) this->cpuInfo.features.push_back("SSE4.2");
    if (cpuInfoArray[2] & (1 << 28)) this->cpuInfo.features.push_back("AVX");
}

void SystemInfo::GatherMemoryInfo() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    
    memoryInfo.totalPhysicalMemoryMB = memInfo.ullTotalPhys / (1024 * 1024);
    memoryInfo.availablePhysicalMemoryMB = memInfo.ullAvailPhys / (1024 * 1024);
    memoryInfo.totalVirtualMemoryMB = memInfo.ullTotalVirtual / (1024 * 1024);
    memoryInfo.availableVirtualMemoryMB = memInfo.ullAvailVirtual / (1024 * 1024);
}

void SystemInfo::GatherOSInfo() {
    OSVERSIONINFOEX osInfo;
    ZeroMemory(&osInfo, sizeof(OSVERSIONINFOEX));
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    
    #pragma warning(disable: 4996)  // Disable GetVersionEx deprecation warning
    GetVersionEx((OSVERSIONINFO*)&osInfo);
    #pragma warning(default: 4996)
    
    std::stringstream ss;
    ss << "Windows ";
    
    if (osInfo.dwMajorVersion == 10) {
        ss << "10";
    } else if (osInfo.dwMajorVersion == 6) {
        switch (osInfo.dwMinorVersion) {
            case 3: ss << "8.1"; break;
            case 2: ss << "8"; break;
            case 1: ss << "7"; break;
            case 0: ss << "Vista"; break;
        }
    }
    
    ss << " (Build " << osInfo.dwBuildNumber << ")";
    
    if (sizeof(void*) == 8) {
        ss << " 64-bit";
    } else {
        ss << " 32-bit";
    }
    
    this->osInfo = ss.str();
} 