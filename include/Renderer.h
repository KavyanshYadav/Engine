#pragma once 

#include "Window.h"
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Scene/Scene.h"
#include "Scene/Mesh.h"
#include "Scene/MeshFactory.h"
#include "Scene/Material.h"
#include <vector>
#include "Input.h"
#include <glad/glad.h>

class Input;  // Forward declaration

class Renderer {
public:
    Renderer(Window* window);
    ~Renderer();

    void Clear();
	void RenderTriangle();
    // void RenderImgui();
    void ReloadShader();
    Scene* getActiveScene();
    Shader* GetShader() { return shader; }
    int *PanelX,*PanelY;

    // Axis lines
    void InitializeAxisLines();
    void RenderAxisLines();
    void CleanupAxisLines();

private:
    GLuint VAO, VBO;
    GLuint shaderProgram;
	Shader* shader;
    void setuptriangle();
    std::vector<Scene*> Scenes;
    GLFWwindow* window;
    Window * activeWindowClass;
    Input* input;  // Add Input member

    // Axis lines buffers
    struct {
        GLuint VAO, VBO;
        Shader* shader;
        bool initialized = false;
    } axisLines;
};

