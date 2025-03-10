#include "Application.h"
#include <iostream>

Application::Application() {
    window = new Window(1280, 720, "OpenGL Engine");
	renderer = new Renderer(window);
    uimanger = new UIManager(window->GetGLFWWindow(),renderer);
    input = new Input(renderer);
    isRunning = true;
}

// Destructor: Clean up resources
Application::~Application() {
    delete renderer;
    delete window;
    delete input;
}

void Application::Run() {
    while (isRunning && !window->ShouldClose()) {
        ProcessInput();
        Update();
        Render();
    }
}

void Application::ProcessInput() {
    window->PollEvents();
    input->UpdateCameraMovement(window->GetGLFWWindow());
}

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
