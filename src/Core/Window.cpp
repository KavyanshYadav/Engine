#include "Window.h"
#include <iostream>


Window::Window(int width, int height, const std::string& title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window,MouseCallback);
    
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::SwapBuffers() {
    glfwSwapBuffers(window);
}

GLFWwindow* Window::GetGLFWWindow() const {
    return window;
}
