#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP
#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"


class UIManager {
public:
    UIManager(GLFWwindow* window,Renderer* renderer);
    ~UIManager();

    void Update();
    void Render();
private:
    Renderer * renderer;
};

#endif // UIMANAGER_HPP
