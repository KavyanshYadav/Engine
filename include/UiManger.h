#pragma once

#include <glad/glad.h>  
#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Scene/Scene.h"
#include "Scene/SceneObject.h"
#include "Scene/MeshFactory.h"
#include "Logger.h"
#include "Core/ConfigManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include <Windows.h>
#include <filesystem>

class UIManager {
public:
    UIManager(GLFWwindow* window, Renderer* renderer);
    ~UIManager();

    void Update();
    void Render();

private:
    void RenderMenuBar();        // New method for menu bar
    void RenderPropertiesPanel();
    void RenderTimelinePanel();
    void RenderSceneHierarchy();
    void RenderCameraControls();  // New method for camera controls
    void RenderDebugPanel();      // New method for debug info
    void DrawCameraGizmo();      // New method for camera gizmo
    void RenderToolbar();      // New method for toolbar
    void RenderSystemInfoWindow();  // New method for system info
    void RenderPerformanceWindow(); // New method for performance monitoring
    
    // New timeline tab methods
    void RenderTimelineTabs();
    void RenderTerminalTab();
    void RenderAnimationTab();
    
    // Performance monitoring structures
    struct CPUMetrics {
        float usagePercent = 0.0f;
        int numThreadsActive = 0;
        float clockSpeed = 0.0f;
        float temperature = 0.0f;
        std::vector<float> usageHistory;
        std::vector<float> temperatureHistory;
    };

    struct GPUMetrics {
        float usagePercent = 0.0f;
        float vramTotalMB = 0.0f;
        float vramUsageMB = 0.0f;
        float clockSpeed = 0.0f;
        float temperature = 0.0f;
        std::vector<float> usageHistory;
        std::vector<float> vramHistory;
    };

    struct MemoryMetrics {
        float totalPhysicalMB = 0.0f;
        float usedPhysicalMB = 0.0f;
        float totalVirtualMB = 0.0f;
        float usedVirtualMB = 0.0f;
        float pageFileUsage = 0.0f;
        std::vector<float> physicalHistory;
        std::vector<float> virtualHistory;
    };

    struct RenderingMetrics {
        float fps = 0.0f;
        float frameTime = 0.0f;
        float gpuFrameTime = 0.0f;
        float cpuFrameTime = 0.0f;
        std::vector<float> fpsHistory;
        std::vector<float> frameTimeHistory;
    };

    struct RenderingStatistics {
        int drawCalls = 0;
        int triangles = 0;
        int vertices = 0;
        int textures = 0;
        int shaderSwitches = 0;
        float batchedDrawCalls = 0.0f;
        float culledObjects = 0.0f;
    };

    struct PerformanceMetrics {
        CPUMetrics cpu;
        GPUMetrics gpu;
        MemoryMetrics memory;
        RenderingMetrics rendering;
        RenderingStatistics statistics;
    };

    struct PerformanceState {
        bool showPerformanceWindow = false;
        bool pauseCollection = false;
        bool showCPUGraph = true;
        bool showGPUGraph = true;
        bool showMemoryGraph = true;
        bool showFPSGraph = true;
        float updateInterval = 0.5f;  // Update interval in seconds
        float timeSinceLastUpdate = 0.0f;
        size_t historySize = 120;  // 1 minute of history at 0.5s intervals
        PerformanceMetrics metrics;
    };

    PerformanceState performanceState;

    void UpdatePerformanceMetrics(float deltaTime);
    void CollectPerformanceMetrics();
    void RenderPerformanceGraph(const char* label, const std::vector<float>& data, 
                              float minScale, float maxScale, const ImVec4& color);
    
    Renderer* renderer;
    GLFWwindow* window;
    float propertyPanelWidth = 300.0f;    // Width of left panel
    float timelinePanelHeight = 200.0f;   // Height of bottom panel
    float toolbarHeight = 40.0f;          // Height of toolbar
    ImVec2 viewportPos;                   // Viewport position
    ImVec2 viewportSize;                  // Viewport size
    
    // Camera gizmo state
    float cameraGizmoSize = 100.0f;       // Size of the camera gizmo
    ImVec2 gizmoPos;                      // Position of the gizmo

    // Debug and rendering states
    struct {
        bool showSystemInfo = false;  // New flag for system info window
    } uiState;

    // Debug state
    struct {
        bool showDebugPanel = false;
        bool wireframeMode = false;
        bool showBoundingBoxes = false;
        bool showNormals = false;
        bool showGrid = true;
        bool showVertexPoints = false;
        float gridSize = 1.0f;
        int gridLines = 20;
    } debugState;

    // Terminal state
    struct {
        bool autoScroll = true;
        bool showDebugLogs = true;
        bool showInfoLogs = true;
        bool showWarningLogs = true;
        bool showCriticalLogs = true;
        ImGuiTextFilter filter;
    } terminalState;

    // File system panel
    struct FileSystemState {
        std::string currentPath = std::filesystem::current_path().string();
        std::vector<std::filesystem::directory_entry> currentEntries;
        std::string selectedFile;
        bool showHiddenFiles = false;
        std::string searchFilter;
        std::vector<std::string> favoriteDirectories;
        ImGuiTextFilter filter;
    } fileSystemState;

    void RenderFileSystemPanel();
    void RefreshDirectoryContents();
    void RenderFileIcon(const std::filesystem::directory_entry& entry);
    void HandleFileDrop();
    std::string GetFileIcon(const std::filesystem::path& path);
    void NavigateToDirectory(const std::string& path);
    
    // Split the timeline panel into two sections
    float fileSystemPanelWidth = 300.0f;  // Width of the file system panel
};
