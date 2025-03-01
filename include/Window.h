#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Input;

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool ShouldClose() const;
    void PollEvents();
    void SwapBuffers();
    GLFWwindow* GetGLFWWindow() const;
    glm::vec2 getWindowSize();
    void setWindowSize(const glm::vec2& size);
    glm::vec2 getFramebufferSize();
    void setTitle(const std::string& title);
    void setFullscreen(bool fullscreen);
    bool isFullscreen() const;
    void minimize();
    void maximize();
    void restore();
    bool shouldClose() const;
    void setShouldClose(bool close);


private:
    GLFWwindow* window;
    Input * inputhandler;
};


