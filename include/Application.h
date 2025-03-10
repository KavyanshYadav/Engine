#pragma once

#include "Window.h"
#include "Renderer.h"
#include "UiManger.h"
#include "Input.h"
#include "Core/WindowManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Application {
public:
    Application();
    ~Application();

    void Run();  // Main loop
    bool IsRunning() const { return isRunning; }
    std::string GetProjectPath() const { return projectPath; }

private:
    void ProcessInput();
    void Update();
    void Render();
    void InitializeImGui();
    void ShutdownImGui();
    void BeginImGuiFrame();
    void EndImGuiFrame();
    void RenderImGui();

    Renderer* renderer;
    UIManager* uimanger;
    Input* input;
    bool isRunning;
    std::string projectPath;
};

