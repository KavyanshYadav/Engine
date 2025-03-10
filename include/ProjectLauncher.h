#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <vector>
#include <filesystem>

struct RecentProject {
    std::string name;
    std::string path;
    std::string lastOpened;
};

class ProjectLauncher {
public:
    ProjectLauncher(GLFWwindow* window);
    ~ProjectLauncher();

    void Show();
    bool ShouldClose() const { return shouldClose; }
    std::string GetSelectedProjectPath() const { return selectedProjectPath; }

private:
    void InitializeImGui();
    void ShutdownImGui();
    void BeginFrame();
    void EndFrame();
    void RenderNewProjectWindow();
    void RenderOpenProjectWindow();
    void RenderRecentProjects();
    void LoadRecentProjects();
    void SaveRecentProjects();
    void AddRecentProject(const std::string& path);
    void RemoveRecentProject(const std::string& path);

    GLFWwindow* window;
    bool shouldClose = false;
    bool showNewProject = false;
    bool showOpenProject = false;
    std::string selectedProjectPath;
    std::vector<RecentProject> recentProjects;
    char newProjectName[256] = "";
    char newProjectPath[1024] = "";
}; 