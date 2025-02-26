#include "Application.h"
#include <iostream>

// Constructor: Initialize subsystems
Application::Application() {
    window = new Window(1280, 720, "OpenGL Engine");
	renderer = new Renderer(window->GetGLFWWindow());
    uimanger = new UIManager(window->GetGLFWWindow(),renderer);
    isRunning = true;
}

// Destructor: Clean up resources
Application::~Application() {
    delete renderer;
    delete window;
}

// Main loop
void Application::Run() {
    while (isRunning && !window->ShouldClose()) {
        ProcessInput();
        Update();
        Render();
    }
}

// Process user input
void Application::ProcessInput() {
    window->PollEvents();
}

// Update scene and UI
void Application::Update() {
    uimanger->Update();
}

// Render the scene
void Application::Render() {
    renderer->Clear();
	renderer->RenderTriangle();
    uimanger->Render();
    // renderer->RenderImgui();

    window->SwapBuffers();
}
