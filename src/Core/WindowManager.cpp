#include "Core/WindowManager.h"
#include "Input.h"
#include <iostream>
#include <glad/glad.h>

WindowManager& WindowManager::GetInstance() {
    static WindowManager instance;
    return instance;
}

void WindowManager::Initialize() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void WindowManager::Shutdown() {
    launcher.reset();
    mainWindow.reset();
    projectLauncher.reset();
    glfwTerminate();
}

void WindowManager::CreateLauncherWindow() {
    if (launcher) {
        std::cerr << "Launcher window already exists" << std::endl;
        return;
    }

    launcher = std::make_unique<Window>(800, 600, "Project Launcher");
    projectLauncher = std::make_unique<ProjectLauncher>(launcher->GetGLFWWindow());
    
    launcher->SetKeyCallback(Input::HandleKeyboardInput);
    launcher->SetMouseButtonCallback(Input::HandleObjectSelection);
    launcher->SetCursorPosCallback(Input::HandleCameraRotation);
    launcher->SetScrollCallback(Input::HandleCameraZoom);
    
    // Make launcher window current context
    glfwMakeContextCurrent(launcher->GetGLFWWindow());
    
    // Initialize GLAD for launcher window
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD for launcher window" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void WindowManager::CreateMainWindow() {
    if (mainWindow) {
        std::cerr << "Main window already exists" << std::endl;
        return;
    }

    mainWindow = std::make_unique<Window>(1280, 720, "Engine");
    mainWindow->SetKeyCallback(Input::HandleKeyboardInput);
    mainWindow->SetMouseButtonCallback(Input::HandleObjectSelection);
    mainWindow->SetCursorPosCallback(Input::HandleCameraRotation);
    mainWindow->SetScrollCallback(Input::HandleCameraZoom);
    
    // Make main window current context
    glfwMakeContextCurrent(mainWindow->GetGLFWWindow());
    
    // Initialize GLAD for main window
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD for main window" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void WindowManager::CloseCurrentWindow() {
    launcher.reset();
    mainWindow.reset();
}

bool WindowManager::ShowProjectLauncher() {
    if (!launcher) {
        std::cerr << "Launcher window not created" << std::endl;
        return false;
    }

    while (!launcher->ShouldClose()) {
        launcher->PollEvents();
        launcher->SwapBuffers();
    }

    return true;
}

Window* WindowManager::GetCurrentWindow() const {
    return mainWindow ? mainWindow.get() : launcher.get();
}

GLFWwindow* WindowManager::GetGLFWWindow() const {
    return mainWindow ? mainWindow->GetGLFWWindow() : (launcher ? launcher->GetGLFWWindow() : nullptr);
}

std::string WindowManager::GetSelectedProjectPath() const {
    return projectLauncher ? projectLauncher->GetSelectedProjectPath() : "";
} 