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
   static float moveSpeed;     // Movement speed in units per second
   static float lastFrame;     // Last frame time for movement calculations
   static Input* instance;  // Static instance pointer
public:
    Input(Renderer *renderer);
    ~Input();

    static void HandleObjectSelection(GLFWwindow* window, int button, int action, int mods);
    static void HandleCameraZoom(GLFWwindow* window, double xoffset, double yoffset);
    static void HandleCameraRotation(GLFWwindow* window, double xpos, double ypos);
    static void HandleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void InitializeInputCallbacks(GLFWwindow* window);
    void UpdateCameraMovement(GLFWwindow* window);
};




