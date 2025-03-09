#include "UiManger.h"

UIManager::UIManager(GLFWwindow* window, Renderer* renderer) : window(window), renderer(renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;
}

void UIManager::Update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Get window size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Calculate viewport area (area between panels)
    viewportPos = ImVec2(propertyPanelWidth, 0);
    viewportSize = ImVec2(width - propertyPanelWidth, height - timelinePanelHeight);

    // Update renderer viewport position and size
    *renderer->PanelX = propertyPanelWidth;
    *renderer->PanelY = timelinePanelHeight;

    // Calculate gizmo position (top-right corner of viewport)
    gizmoPos = ImVec2(width - cameraGizmoSize - 10, 10);

    // Properties Panel (Left)
    RenderPropertiesPanel();

    // Camera Controls
    RenderCameraControls();

    // Timeline Panel (Bottom)
    RenderTimelinePanel();

    // Draw camera gizmo
    DrawCameraGizmo();
}

void UIManager::RenderPropertiesPanel() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(propertyPanelWidth, ImGui::GetIO().DisplaySize.y));
    ImGui::Begin("Scene", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse);

    // Scene Hierarchy section
    if (ImGui::CollapsingHeader("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto scene = renderer->getActiveScene();
        int nodeId = 0;
        for (auto* obj : scene->GetSceneNodes()) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            
            // Check if this node is selected
            if (obj == scene->GetActiveMesh()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }

            bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)nodeId, flags, "Node %d", nodeId);
            
            // Handle selection
            if (ImGui::IsItemClicked()) {
                scene->SetActiveMesh(obj);
            }

            if (nodeOpen) {
                ImGui::Text("Mesh: %p", obj);
                ImGui::TreePop();
            }
            nodeId++;
        }
    }

    // Properties section - only show if something is selected
    auto scene = renderer->getActiveScene();
    SceneObject* activeMesh = scene->GetActiveMesh();

    if (activeMesh && ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto mesh = static_cast<Mesh*>(activeMesh);
        
        // Position
        glm::vec3 position = mesh->GetPosition();
        if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f)) {
            mesh->SetPosition(position);
        }

        // Scale
        glm::vec3 scale = mesh->GetScale();
        if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f)) {
            mesh->SetScale(scale);
        }

        // Add a button to focus on selected object
        if (ImGui::Button("Focus")) {
            // Move camera to look at selected object
            glm::vec3 newCameraPos = position - glm::vec3(0.0f, 0.0f, 5.0f);
            scene->SetCameraView(newCameraPos, position, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // Add delete button
        if (ImGui::Button("Delete")) {
            scene->RemoveObject(activeMesh);
            scene->SetActiveMesh(nullptr);
        }
    }

    ImGui::End();
}

void UIManager::RenderTimelinePanel() {
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - timelinePanelHeight));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, timelinePanelHeight));
    ImGui::Begin("Timeline", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse);
    
    ImGui::Text("Timeline Panel");
    
    ImGui::End();
}

void UIManager::RenderCameraControls() {
    ImGui::SetNextWindowPos(ImVec2(propertyPanelWidth + 10, 10));
    ImGui::SetNextWindowBgAlpha(0.7f);
    if (ImGui::Begin("Camera Controls", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings)) {
        
        Scene* scene = renderer->getActiveScene();
        
        if (ImGui::Button("Reset Camera")) {
            // Reset to default camera position
            scene->SetCameraView(
                glm::vec3(0.0f, 0.0f, 3.0f),  // Position
                glm::vec3(0.0f, 0.0f, 0.0f),  // Target
                glm::vec3(0.0f, 1.0f, 0.0f)   // Up
            );
        }

        glm::vec3 camPos = scene->GetCameraPosition();
        if (ImGui::DragFloat3("Position", glm::value_ptr(camPos), 0.1f)) {
            scene->SetCameraView(camPos, 
                scene->GetCameraPosition() + glm::normalize(scene->GetCameraTarget() - scene->GetCameraPosition()),
                glm::vec3(0.0f, 1.0f, 0.0f));
        }

        if (ImGui::Button("Top View")) {
            scene->SetCameraView(
                glm::vec3(0.0f, 5.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, -1.0f)
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("Front View")) {
            scene->SetCameraView(
                glm::vec3(0.0f, 0.0f, 5.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("Side View")) {
            scene->SetCameraView(
                glm::vec3(5.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
    }
    ImGui::End();
}

void UIManager::DrawCameraGizmo() {
    ImGui::SetNextWindowPos(gizmoPos);
    ImGui::SetNextWindowBgAlpha(0.5f);
    if (ImGui::Begin("Camera Gizmo", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings)) {
        
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 center = ImVec2(gizmoPos.x + cameraGizmoSize/2, gizmoPos.y + cameraGizmoSize/2);
        
        // Get camera orientation from scene
        Scene* scene = renderer->getActiveScene();
        glm::vec3 camDir = glm::normalize(scene->GetCameraTarget() - scene->GetCameraPosition());
        glm::vec3 right = glm::normalize(glm::cross(camDir, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::normalize(glm::cross(right, camDir));
        
        float axisLength = cameraGizmoSize * 0.4f;
        
        // Draw X axis (right) - Red
        draw_list->AddLine(
            center,
            ImVec2(center.x + right.x * axisLength, center.y - right.z * axisLength),
            IM_COL32(255, 0, 0, 255), 2.0f);
        
        // Draw Y axis (up) - Green
        draw_list->AddLine(
            center,
            ImVec2(center.x, center.y - axisLength),
            IM_COL32(0, 255, 0, 255), 2.0f);
        
        // Draw Z axis (forward) - Blue
        draw_list->AddLine(
            center,
            ImVec2(center.x + camDir.x * axisLength, center.y - camDir.z * axisLength),
            IM_COL32(0, 0, 255, 255), 2.0f);
        
        // Labels
        draw_list->AddText(ImVec2(center.x + right.x * axisLength, center.y - right.z * axisLength), 
            IM_COL32(255, 0, 0, 255), "X");
        draw_list->AddText(ImVec2(center.x, center.y - axisLength), 
            IM_COL32(0, 255, 0, 255), "Y");
        draw_list->AddText(ImVec2(center.x + camDir.x * axisLength, center.y - camDir.z * axisLength), 
            IM_COL32(0, 0, 255, 255), "Z");
    }
    ImGui::End();
}

void UIManager::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UIManager::~UIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
