#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool ShouldClose() const;
    void PollEvents();
    void SwapBuffers();
    GLFWwindow* GetGLFWWindow() const;

private:
    GLFWwindow* window;
};

#endif
