#include "Application.h"
#include <iostream>

Application::Application() {
    auto& windowManager = WindowManager::GetInstance();
    windowManager.Initialize();
    
    // Create and show project launcher
    windowManager.CreateLauncherWindow();
    if (!windowManager.ShowProjectLauncher()) {
        isRunning = false;
        return;
    }

    // Get selected project path
    projectPath = windowManager.GetSelectedProjectPath();
    if (projectPath.empty()) {
        isRunning = false;
        return;
    }

    // Create main application window
    windowManager.CreateMainWindow();
    
    // Initialize OpenGL context
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        isRunning = false;
        return;
    }

    // Initialize components
    renderer = new Renderer(windowManager.GetCurrentWindow());
    uimanger = new UIManager(windowManager.GetGLFWWindow(), renderer);
    input = new Input(renderer);
    
    // Initialize ImGui
    InitializeImGui();
    
    isRunning = true;
}

// Destructor: Clean up resources
Application::~Application() {
    ShutdownImGui();
    delete renderer;
    delete uimanger;
    delete input;
    WindowManager::GetInstance().Shutdown();
}

void Application::InitializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(WindowManager::GetInstance().GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void Application::ShutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::BeginImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Application::EndImGuiFrame() {
    ImGui::Render();
}

void Application::RenderImGui() {
    int display_w, display_h;
    glfwGetFramebufferSize(WindowManager::GetInstance().GetGLFWWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::Run() {
    auto& windowManager = WindowManager::GetInstance();
    
    while (isRunning && !windowManager.GetCurrentWindow()->ShouldClose()) {
        ProcessInput();
        Update();
        Render();
    }
}

void Application::ProcessInput() {
    auto& windowManager = WindowManager::GetInstance();
    windowManager.GetCurrentWindow()->PollEvents();
    input->UpdateCameraMovement(windowManager.GetGLFWWindow());
}

void Application::Update() {
    uimanger->Update();
}

// Render the scene
void Application::Render() {
    auto& windowManager = WindowManager::GetInstance();
    
    renderer->Clear();
    renderer->RenderTriangle();
    uimanger->Render();
    
    BeginImGuiFrame();
    EndImGuiFrame();
    RenderImGui();
    
    windowManager.GetCurrentWindow()->SwapBuffers();
}
