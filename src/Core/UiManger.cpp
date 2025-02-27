#include "UiManger.h"

UIManager::UIManager(GLFWwindow * window,Renderer* renderer )
{
    this->renderer = renderer;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io; // Optional: silences unused variable warnings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

UIManager::~UIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::Update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::Render() {
    ImGui::Begin("Debug Info");
    ImGui::Text("OpenGL Rendering Engine");
    if (ImGui::Button("Recompile Shader")) {
        renderer->ReloadShader();
    }
    if (ImGui::Button("Get Scene Meshes")) {
        for (SceneObject* obj: renderer->getActiveScene()->GetSceneNodes())
    {
        std::cout << obj ; 
    }
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0)); // Lock position
    ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Settings");
    ImGui::End();

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
