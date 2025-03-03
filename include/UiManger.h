#pragma once

#include <glad/glad.h>  
#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Scene/Scene.h"
#include "Scene/SceneObject.h"


class UIManager {
public:
    UIManager(GLFWwindow* window,Renderer* renderer);
    ~UIManager();

    void Update();
    void Render();
private:
    Renderer * renderer;
};
