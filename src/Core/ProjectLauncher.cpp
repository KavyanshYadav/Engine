#include "ProjectLauncher.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

ProjectLauncher::ProjectLauncher(GLFWwindow* window) : window(window) {
    LoadRecentProjects();
    InitializeImGui();
}

ProjectLauncher::~ProjectLauncher() {
    SaveRecentProjects();
    ShutdownImGui();
}

void ProjectLauncher::InitializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void ProjectLauncher::ShutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ProjectLauncher::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ProjectLauncher::EndFrame() {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ProjectLauncher::Show() {
    BeginFrame();

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Project Launcher", nullptr, ImGuiWindowFlags_NoCollapse);

    // Main menu buttons
    if (ImGui::Button("New Project", ImVec2(200, 50))) {
        showNewProject = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Open Project", ImVec2(200, 50))) {
        showOpenProject = true;
    }

    // Recent Projects section
    ImGui::Separator();
    ImGui::Text("Recent Projects");
    RenderRecentProjects();

    ImGui::End();

    // Modal windows
    if (showNewProject) {
        RenderNewProjectWindow();
    }
    if (showOpenProject) {
        RenderOpenProjectWindow();
    }

    EndFrame();
}

void ProjectLauncher::RenderNewProjectWindow() {
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
    ImGui::Begin("New Project", &showNewProject, ImGuiWindowFlags_Modal);

    ImGui::InputText("Project Name", newProjectName, sizeof(newProjectName));
    ImGui::InputText("Project Path", newProjectPath, sizeof(newProjectPath));
    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
        // TODO: Implement file browser
    }

    if (ImGui::Button("Create Project")) {
        if (strlen(newProjectName) > 0 && strlen(newProjectPath) > 0) {
            std::filesystem::path projectPath(newProjectPath);
            if (!std::filesystem::exists(projectPath)) {
                std::filesystem::create_directories(projectPath);
            }
            AddRecentProject(newProjectPath);
            selectedProjectPath = newProjectPath;
            shouldClose = true;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        showNewProject = false;
    }

    ImGui::End();
}

void ProjectLauncher::RenderOpenProjectWindow() {
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
    ImGui::Begin("Open Project", &showOpenProject, ImGuiWindowFlags_Modal);

    // TODO: Implement file browser
    if (ImGui::Button("Browse")) {
        // TODO: Implement file browser
    }

    if (ImGui::Button("Open")) {
        if (strlen(newProjectPath) > 0) {
            AddRecentProject(newProjectPath);
            selectedProjectPath = newProjectPath;
            shouldClose = true;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        showOpenProject = false;
    }

    ImGui::End();
}

void ProjectLauncher::RenderRecentProjects() {
    for (const auto& project : recentProjects) {
        ImGui::BeginChild(project.path.c_str(), ImVec2(0, 60), true);
        
        ImGui::Text(project.name.c_str());
        ImGui::Text(project.path.c_str());
        ImGui::Text("Last opened: %s", project.lastOpened.c_str());
        
        if (ImGui::Button(("Open##" + project.path).c_str())) {
            selectedProjectPath = project.path;
            shouldClose = true;
        }
        ImGui::SameLine();
        if (ImGui::Button(("Remove##" + project.path).c_str())) {
            RemoveRecentProject(project.path);
        }
        
        ImGui::EndChild();
    }
}

void ProjectLauncher::LoadRecentProjects() {
    std::ifstream file("recent_projects.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('|');
        if (pos != std::string::npos) {
            RecentProject project;
            project.name = line.substr(0, pos);
            project.path = line.substr(pos + 1);
            std::getline(file, project.lastOpened);
            recentProjects.push_back(project);
        }
    }
}

void ProjectLauncher::SaveRecentProjects() {
    std::ofstream file("recent_projects.txt");
    if (!file.is_open()) return;

    for (const auto& project : recentProjects) {
        file << project.name << "|" << project.path << "\n";
        file << project.lastOpened << "\n";
    }
}

void ProjectLauncher::AddRecentProject(const std::string& path) {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    RecentProject project;
    project.name = std::filesystem::path(path).stem().string();
    project.path = path;
    project.lastOpened = oss.str();

    // Remove if already exists
    RemoveRecentProject(path);

    // Add to beginning
    recentProjects.insert(recentProjects.begin(), project);

    // Keep only last 10 projects
    if (recentProjects.size() > 10) {
        recentProjects.pop_back();
    }
}

void ProjectLauncher::RemoveRecentProject(const std::string& path) {
    recentProjects.erase(
        std::remove_if(recentProjects.begin(), recentProjects.end(),
            [&path](const RecentProject& p) { return p.path == path; }),
        recentProjects.end()
    );
} 