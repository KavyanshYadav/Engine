#pragma once

#include <glad/glad.h>  
#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Scene/Scene.h"
#include "Scene/SceneObject.h"
#include <glm/gtc/type_ptr.hpp>

class UIManager {
public:
    UIManager(GLFWwindow* window, Renderer* renderer);
    ~UIManager();

    void Update();
    void Render();

private:
    void RenderPropertiesPanel();
    void RenderTimelinePanel();
    void RenderSceneHierarchy();
    void RenderCameraControls();  // New method for camera controls
    void DrawCameraGizmo();      // New method for camera gizmo
    
    Renderer* renderer;
    GLFWwindow* window;
    float propertyPanelWidth = 300.0f;    // Width of left panel
    float timelinePanelHeight = 200.0f;   // Height of bottom panel
    ImVec2 viewportPos;                   // Viewport position
    ImVec2 viewportSize;                  // Viewport size
    
    // Camera gizmo state
    float cameraGizmoSize = 100.0f;       // Size of the camera gizmo
    ImVec2 gizmoPos;                      // Position of the gizmo
};
