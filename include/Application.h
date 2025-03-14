#pragma once

#include "Window.h"
#include "Renderer.h"
#include "UiManger.h"
#include "Input.h"

class Application {
public:
    Application();
    ~Application();

    void Run();  // Main loop

private:
    Window* window;
    Renderer* renderer;
    UIManager * uimanger;
    Input* input;
    bool isRunning;

    void ProcessInput();
    void Update();
    void Render();
};

