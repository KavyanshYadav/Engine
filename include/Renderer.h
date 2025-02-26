#pragma once 


#include <GLFW/glfw3.h>
#include "Shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Scene/Scene.h"
#include "Scene/Mesh.h"
#include <vector>


class Renderer {
public:
    Renderer();
    Renderer(GLFWwindow* window);
    ~Renderer();

    void Clear();
	void RenderTriangle();
    void RenderImgui();
    void ReloadShader();
private:
    GLuint VAO, VBO;
    GLuint shaderProgram;
	Shader* shader;
    void setuptriangle();
    std::vector<Scene*> Scenes;
};

