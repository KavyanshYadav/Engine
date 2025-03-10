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
    
    // New timeline tab methods
    void RenderTimelineTabs();
    void RenderTerminalTab();
    void RenderAnimationTab();
    
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
};
