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
#include <glm/glm.hpp>

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

    // Background color control
    void SetBackgroundColor(const glm::vec3& color) { backgroundColor = color; }
    const glm::vec3& GetBackgroundColor() const { return backgroundColor; }

    // Axis lines
    void InitializeAxisLines();
    void RenderAxisLines();
    void CleanupAxisLines();

    // Statistics getters
    int GetDrawCalls() const { return drawCalls; }
    int GetTriangleCount() const { return triangleCount; }
    int GetVertexCount() const { return vertexCount; }
    int GetTextureCount() const { return textureCount; }
    int GetShaderSwitches() const { return shaderSwitches; }

private:
    GLuint VAO, VBO;
    GLuint shaderProgram;
	Shader* shader;
    void setuptriangle();
    std::vector<Scene*> Scenes;
    GLFWwindow* window;
    Window * activeWindowClass;
    Input* input;  // Add Input member

    // Background color
    glm::vec3 backgroundColor = glm::vec3(0.2f, 0.2f, 0.2f);  // Default dark gray

    // Axis lines buffers
    struct {
        GLuint VAO, VBO;
        Shader* shader;
        bool initialized = false;
    } axisLines;

    // Rendering statistics
    int drawCalls = 0;
    int triangleCount = 0;
    int vertexCount = 0;
    int textureCount = 0;
    int shaderSwitches = 0;
};

