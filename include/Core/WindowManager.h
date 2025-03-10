#pragma once
#include "Window.h"
#include "ProjectLauncher.h"
#include <memory>

class Input;  // Forward declaration

class WindowManager {
public:
    static WindowManager& GetInstance();
    
    void Initialize();
    void Shutdown();
    
    // Window management
    void CreateLauncherWindow();
    void CreateMainWindow();
    void CloseCurrentWindow();
    
    // Project launcher
    bool ShowProjectLauncher();
    std::string GetSelectedProjectPath() const;
    
    // Window access
    Window* GetCurrentWindow() const { return currentWindow.get(); }
    GLFWwindow* GetGLFWWindow() const { return currentWindow ? currentWindow->GetGLFWWindow() : nullptr; }

private:
    WindowManager() = default;
    ~WindowManager() = default;
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;

    std::unique_ptr<Window> launcher;
    std::unique_ptr<Window> mainWindow;
    std::unique_ptr<ProjectLauncher> projectLauncher;
}; 