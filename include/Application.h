#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.h"
#include "Renderer.h"
#include "UiManger.h"

class Application {
public:
    Application();
    ~Application();

    void Run();  // Main loop

private:
    Window* window;
    Renderer* renderer;
    UIManager * uimanger;

    bool isRunning;

    void ProcessInput();
    void Update();
    void Render();
};

#endif 
