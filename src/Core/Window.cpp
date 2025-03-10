#include "Window.h"
#include <iostream>
#include "Input.h"
#include <glad/glad.h>

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
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
}


glm::vec2 Window::getWindowSize(){
    int height,width = 0;
    glfwGetWindowSize(window,&width,&height);
    return glm::vec2(width,height);
}

glm::vec2 Window::getFramebufferSize() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return glm::vec2(width, height);
}

void Window::setWindowSize(const glm::vec2& size) {
    glfwSetWindowSize(window, static_cast<int>(size.x), static_cast<int>(size.y));
}

void Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(window, title.c_str());
}

void Window::setFullscreen(bool fullscreen) {
    if (fullscreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(window, nullptr, 100, 100, 800, 600, GLFW_DONT_CARE);
    }
}

bool Window::isFullscreen() const {
    return glfwGetWindowMonitor(window) != nullptr;
}

void Window::minimize() {
    glfwIconifyWindow(window);
}

void Window::maximize() {
    glfwMaximizeWindow(window);
}

void Window::restore() {
    glfwRestoreWindow(window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool close) {
    glfwSetWindowShouldClose(window, close);
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

void Window::SetKeyCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(window, callback);
}

void Window::SetMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(window, callback);
}

void Window::SetCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(window, callback);
}

void Window::SetScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(window, callback);
}
