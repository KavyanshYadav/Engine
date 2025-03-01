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
public:
    Input(Renderer *renderer);
    ~Input();

    void MouseCallback(GLFWwindow* window, int button, int action, int mods);

};




