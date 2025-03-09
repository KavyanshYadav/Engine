#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene/Mesh.h"
#include "Utiles.h"
#include "Renderer.h"


class Renderer;

class Input
{
private:
   Renderer * renderer;
   static bool firstMouse;
   static float lastX, lastY;
   static float mouseSensitivity;
public:
    Input(Renderer *renderer);
    ~Input();

    void MouseCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void SetupCallbacks(GLFWwindow* window);
    void ProcessInput(GLFWwindow* window);
};




