#include "UiManger.h"
#include <functional>

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

    // Set consistent window flags for all panels
    ImGuiWindowFlags commonFlags = 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    // 1. Menu Bar (top, full width)
    RenderMenuBar();

    // 2. Toolbar (below menu bar, excluding left panel)
    ImGui::SetNextWindowPos(ImVec2(propertyPanelWidth, ImGui::GetFrameHeight())); // Account for menu bar height
    ImGui::SetNextWindowSize(ImVec2(width - propertyPanelWidth, toolbarHeight));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    RenderToolbar();
    ImGui::PopStyleVar();

    // 3. Properties Panel (left side, full height except menu bar)
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    ImGui::SetNextWindowSize(ImVec2(propertyPanelWidth, height - ImGui::GetFrameHeight()));
    RenderPropertiesPanel();

    // 4. Timeline Panel (bottom, excluding left panel)
    ImGui::SetNextWindowPos(ImVec2(propertyPanelWidth, height - timelinePanelHeight));
    ImGui::SetNextWindowSize(ImVec2(width - propertyPanelWidth, timelinePanelHeight));
    RenderTimelinePanel();

    // Calculate viewport area (area between panels)
    viewportPos = ImVec2(propertyPanelWidth, toolbarHeight + ImGui::GetFrameHeight());
    viewportSize = ImVec2(
        width - propertyPanelWidth,
        height - timelinePanelHeight - toolbarHeight - ImGui::GetFrameHeight()
    );

    // Update renderer viewport position and size
    *renderer->PanelX = propertyPanelWidth;
    *renderer->PanelY = timelinePanelHeight;

    // 5. Camera Controls (overlay on viewport, bottom right)
    float camControlWidth = 200.0f;
    float camControlHeight = 120.0f;
    ImGui::SetNextWindowPos(ImVec2(
        width - camControlWidth - 10,
        height - timelinePanelHeight - camControlHeight - 10
    ));
    ImGui::SetNextWindowSize(ImVec2(camControlWidth, camControlHeight));
    RenderCameraControls();

    // 6. Camera Gizmo (overlay on viewport, top right)
    gizmoPos = ImVec2(
        width - cameraGizmoSize - 10,
        toolbarHeight + ImGui::GetFrameHeight() + 10
    );
    DrawCameraGizmo();

    // 7. Debug Panel (if enabled, overlay on viewport, right side)
    if (debugState.showDebugPanel) {
        ImGui::SetNextWindowPos(
            ImVec2(width - 300, toolbarHeight + ImGui::GetFrameHeight() + cameraGizmoSize + 20),
            ImGuiCond_FirstUseEver
        );
        ImGui::SetNextWindowSize(ImVec2(280, 400), ImGuiCond_FirstUseEver);
        RenderDebugPanel();
    }
}

void UIManager::RenderMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Import Model...", "Ctrl+I")) {}
            if (ImGui::MenuItem("Export Scene...", "Ctrl+E")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            if (ImGui::MenuItem("Delete", "Del")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Select All", "Ctrl+A")) {}
            if (ImGui::MenuItem("Deselect", "Ctrl+D")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Reset View", "F")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Top View", "Num7")) {}
            if (ImGui::MenuItem("Front View", "Num1")) {}
            if (ImGui::MenuItem("Side View", "Num3")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Viewport")) {
                ImGui::MenuItem("Wireframe", "Z", &debugState.wireframeMode);
                ImGui::MenuItem("Show Grid", "G", &debugState.showGrid);
                ImGui::MenuItem("Show Normals", "N", &debugState.showNormals);
                ImGui::MenuItem("Show Vertices", "V", &debugState.showVertexPoints);
                ImGui::MenuItem("Show Bounding Boxes", "B", &debugState.showBoundingBoxes);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Create")) {
            if (ImGui::BeginMenu("3D Objects")) {
                if (ImGui::MenuItem("Cube")) {}
                if (ImGui::MenuItem("Sphere")) {}
                if (ImGui::MenuItem("Cylinder")) {}
                if (ImGui::MenuItem("Plane")) {}
                if (ImGui::MenuItem("Cone")) {}
                if (ImGui::MenuItem("Torus")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Lights")) {
                if (ImGui::MenuItem("Point Light")) {}
                if (ImGui::MenuItem("Directional Light")) {}
                if (ImGui::MenuItem("Spot Light")) {}
                if (ImGui::MenuItem("Area Light")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Cameras")) {
                if (ImGui::MenuItem("Perspective Camera")) {}
                if (ImGui::MenuItem("Orthographic Camera")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Transform", "W")) {}
            if (ImGui::MenuItem("Rotate", "E")) {}
            if (ImGui::MenuItem("Scale", "R")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Snap to Grid", "Ctrl+Shift+G")) {}
            if (ImGui::MenuItem("Snap to Vertex", "Ctrl+Shift+V")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Scene Settings...")) {}
            if (ImGui::MenuItem("Preferences...")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug")) {
            ImGui::MenuItem("Show Debug Panel", nullptr, &debugState.showDebugPanel);
            ImGui::Separator();
            if (ImGui::MenuItem("Performance Monitor")) {}
            if (ImGui::MenuItem("Scene Statistics")) {}
            if (ImGui::MenuItem("Memory Usage")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Reload Shaders")) {}
            if (ImGui::MenuItem("Clear Cache")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Documentation")) {}
            if (ImGui::MenuItem("Shortcuts")) {}
            if (ImGui::MenuItem("About")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void UIManager::RenderPropertiesPanel() {
    ImGuiWindowFlags propertiesFlags = 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Properties", nullptr, propertiesFlags);

    // Scene Hierarchy section
    if (ImGui::CollapsingHeader("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen)) {
        RenderSceneHierarchy();
    }

    // Properties section - only show if something is selected
    auto scene = renderer->getActiveScene();
    SceneObject* activeMesh = scene->GetActiveMesh();

    if (activeMesh && ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto mesh = static_cast<Mesh*>(activeMesh);
        
        // Name field
        static char nameBuf[256];
        strncpy(nameBuf, mesh->GetName().c_str(), sizeof(nameBuf) - 1);
        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
            mesh->SetName(nameBuf);
        }

        // Transform section
        if (ImGui::TreeNode("Transform")) {
            glm::vec3 position = mesh->GetPosition();
        if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f)) {
            mesh->SetPosition(position);
        }

            glm::vec3 rotation = mesh->GetRotation();
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 1.0f)) {
                mesh->SetRotation(rotation);
            }

            glm::vec3 scale = mesh->GetScale();
            if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f)) {
            mesh->SetScale(scale);
            }

            ImGui::TreePop();
        }

        // Rendering section
        if (ImGui::TreeNode("Rendering")) {
            bool isVisible = mesh->IsVisible();
            if (ImGui::Checkbox("Visible", &isVisible)) {
                mesh->SetVisible(isVisible);
            }

            bool castShadows = mesh->CastShadows();
            if (ImGui::Checkbox("Cast Shadows", &castShadows)) {
                mesh->SetCastShadows(castShadows);
            }

            bool receiveShadows = mesh->ReceiveShadows();
            if (ImGui::Checkbox("Receive Shadows", &receiveShadows)) {
                mesh->SetReceiveShadows(receiveShadows);
            }

            // Layer selection
            const char* layers[] = { "Default", "Background", "Environment", "Props", "Characters", "UI", "Effects" };
            int currentLayer = static_cast<int>(mesh->GetLayer());
            if (ImGui::Combo("Layer", &currentLayer, layers, IM_ARRAYSIZE(layers))) {
                mesh->SetLayer(static_cast<MeshLayer>(currentLayer));
            }

            ImGui::TreePop();
        }

        // Physics section
        if (ImGui::TreeNode("Physics")) {
            bool isStatic = mesh->IsStatic();
            if (ImGui::Checkbox("Static", &isStatic)) {
                mesh->SetStatic(isStatic);
            }

            bool isDynamic = mesh->IsDynamic();
            if (ImGui::Checkbox("Dynamic", &isDynamic)) {
                mesh->SetDynamic(isDynamic);
            }

            if (isDynamic) {
                float mass = mesh->GetMass();
                if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, 1000.0f)) {
                    mesh->SetMass(mass);
                }
            }

            ImGui::TreePop();
        }

        // Animation section
        if (ImGui::TreeNode("Animation")) {
            bool isAnimated = mesh->IsAnimated();
            if (ImGui::Checkbox("Animated", &isAnimated)) {
                mesh->SetAnimated(isAnimated);
            }

            ImGui::TreePop();
        }

        // Statistics section
        if (ImGui::TreeNode("Statistics")) {
            const MeshStats& stats = mesh->GetStats();
            ImGui::Text("Vertices: %d", stats.vertexCount);
            ImGui::Text("Triangles: %d", stats.triangleCount);
            ImGui::Text("Materials: %d", stats.materialCount);
            ImGui::Text("Bounding Sphere Radius: %.2f", stats.boundingSphereRadius);
            ImGui::TreePop();
        }

        // Materials section
        if (ImGui::TreeNode("Materials")) {
            const auto& materials = mesh->GetMaterials();
            for (size_t i = 0; i < materials.size(); i++) {
                if (ImGui::TreeNode(("Material " + std::to_string(i)).c_str())) {
                    // Material properties will be added here
                    ImGui::TreePop();
                }
            }
            if (ImGui::Button("Add Material")) {
                // Material creation logic will be added here
            }
            ImGui::TreePop();
        }

        // Hierarchy section
        if (ImGui::TreeNode("Hierarchy")) {
            Mesh* parent = mesh->GetParent();
            ImGui::Text("Parent: %s", parent ? parent->GetName().c_str() : "None");

            if (ImGui::TreeNode("Children")) {
                const auto& children = mesh->GetChildren();
                for (const auto& child : children) {
                    ImGui::Text("%s", child->GetName().c_str());
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        // Actions
        ImGui::Separator();
        if (ImGui::Button("Focus")) {
            glm::vec3 pos = mesh->GetPosition();
            glm::vec3 newCameraPos = pos - glm::vec3(0.0f, 0.0f, 5.0f);
            scene->SetCameraView(newCameraPos, pos, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        ImGui::SameLine();
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

    RenderTimelineTabs();

    ImGui::End();
}

void UIManager::RenderTimelineTabs() {
    if (ImGui::BeginTabBar("TimelineTabs")) {
        if (ImGui::BeginTabItem("Terminal")) {
            RenderTerminalTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Animation")) {
            RenderAnimationTab();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void UIManager::RenderTerminalTab() {
    // Log filter controls
    ImGui::BeginChild("TerminalControls", ImVec2(0, 30), true);
    
    // Log level toggles
    ImGui::Checkbox("Debug", &terminalState.showDebugLogs);
    ImGui::SameLine();
    ImGui::Checkbox("Info", &terminalState.showInfoLogs);
    ImGui::SameLine();
    ImGui::Checkbox("Warning", &terminalState.showWarningLogs);
    ImGui::SameLine();
    ImGui::Checkbox("Critical", &terminalState.showCriticalLogs);
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &terminalState.autoScroll);
    ImGui::SameLine();
    
    // Clear button
    if (ImGui::Button("Clear")) {
        Logger::GetInstance().ClearLogs();
    }
    ImGui::SameLine();
    
    // Filter
    terminalState.filter.Draw("Filter", 180);
    
    ImGui::EndChild();

    // Log display
    ImGui::BeginChild("LogScroll", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    
    const auto& logs = Logger::GetInstance().GetLogEntries();
    ImGuiListClipper clipper;
    clipper.Begin(logs.size());
    
    while (clipper.Step()) {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
            const auto& entry = logs[i];
            
            // Check if this log level is enabled
            bool showLog = false;
            ImVec4 color(1.0f, 1.0f, 1.0f, 1.0f);
            
            switch (entry.level) {
                case LogLevel::DEBUG:
                    showLog = terminalState.showDebugLogs;
                    color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
                    break;
                case LogLevel::INFO:
                    showLog = terminalState.showInfoLogs;
                    color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    break;
                case LogLevel::WARNING:
                    showLog = terminalState.showWarningLogs;
                    color = ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
                    break;
                case LogLevel::CRITICAL:
                    showLog = terminalState.showCriticalLogs;
                    color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                    break;
            }

            if (showLog && terminalState.filter.PassFilter(entry.message.c_str())) {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                
                // Format timestamp
                auto timePoint = entry.timestamp;
                std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
                std::string timestamp = std::ctime(&time);
                timestamp.pop_back(); // Remove newline
                
                ImGui::TextUnformatted(timestamp.c_str());
                ImGui::SameLine();
                ImGui::TextUnformatted(entry.message.c_str());
                
                ImGui::PopStyleColor();
            }
        }
    }
    
    if (terminalState.autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }
    
    ImGui::EndChild();
}

void UIManager::RenderAnimationTab() {
    ImGui::Text("Animation timeline coming soon...");
}

void UIManager::RenderCameraControls() {
    ImGuiWindowFlags cameraFlags = 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Camera Controls", nullptr, cameraFlags);
    
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

    // Arrange view buttons horizontally
    float buttonWidth = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2) / 3;
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));
    if (ImGui::Button("Top", ImVec2(buttonWidth, 0))) {
        scene->SetCameraView(
            glm::vec3(0.0f, 5.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        );
    }
    ImGui::SameLine();
    if (ImGui::Button("Front", ImVec2(buttonWidth, 0))) {
        scene->SetCameraView(
            glm::vec3(0.0f, 0.0f, 5.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }
    ImGui::SameLine();
    if (ImGui::Button("Side", ImVec2(buttonWidth, 0))) {
        scene->SetCameraView(
            glm::vec3(5.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }
    ImGui::PopStyleVar();

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

void UIManager::RenderToolbar() {
    ImGuiWindowFlags toolbarFlags = 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Toolbar", nullptr, toolbarFlags);

    // Add shape creation buttons
    if (ImGui::Button("Add Cube")) {
        Scene* scene = renderer->getActiveScene();
        Mesh* cube = MeshFactory::CreateCube(renderer->GetShader());
        scene->AddObject(cube);
    }
    ImGui::SameLine();

    if (ImGui::Button("Add Sphere")) {
        Scene* scene = renderer->getActiveScene();
        Mesh* sphere = MeshFactory::CreateSphere(renderer->GetShader());
        scene->AddObject(sphere);
    }
    ImGui::SameLine();

    if (ImGui::Button("Add Plane")) {
        Scene* scene = renderer->getActiveScene();
        Mesh* plane = MeshFactory::CreatePlane(renderer->GetShader());
        scene->AddObject(plane);
    }
    ImGui::SameLine();

    if (ImGui::Button("Add Cylinder")) {
        Scene* scene = renderer->getActiveScene();
        Mesh* cylinder = MeshFactory::CreateCylinder(renderer->GetShader());
        scene->AddObject(cylinder);
    }

    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();

    // Debug options
    if (ImGui::Checkbox("Wireframe", &debugState.wireframeMode)) {
        if (debugState.wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    ImGui::SameLine();

    ImGui::Checkbox("Bounding Boxes", &debugState.showBoundingBoxes);
    ImGui::SameLine();

    ImGui::Checkbox("Normals", &debugState.showNormals);
    ImGui::SameLine();

    ImGui::Checkbox("Grid", &debugState.showGrid);
    ImGui::SameLine();

    ImGui::Checkbox("Vertices", &debugState.showVertexPoints);
    ImGui::SameLine();

    if (ImGui::Checkbox("Debug Info", &debugState.showDebugPanel)) {
        // Toggle debug panel
    }
    
    ImGui::End();

    // Render debug panel if enabled
    if (debugState.showDebugPanel) {
        RenderDebugPanel();
    }
}

void UIManager::RenderDebugPanel() {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, toolbarHeight + 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(280, 400), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Debug Info", &debugState.showDebugPanel)) {
        Scene* scene = renderer->getActiveScene();
        
        if (ImGui::CollapsingHeader("Render Stats", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
            ImGui::Text("Objects in Scene: %d", scene->GetSceneNodes().size());
            
            if (ImGui::TreeNode("Grid Settings")) {
                ImGui::DragFloat("Grid Size", &debugState.gridSize, 0.1f, 0.1f, 10.0f);
                ImGui::DragInt("Grid Lines", &debugState.gridLines, 1, 2, 100);
                ImGui::TreePop();
            }
        }

        if (ImGui::CollapsingHeader("Camera Info", ImGuiTreeNodeFlags_DefaultOpen)) {
            glm::vec3 camPos = scene->GetCameraPosition();
            glm::vec3 camTarget = scene->GetCameraTarget();
            
            ImGui::Text("Position: %.2f, %.2f, %.2f", camPos.x, camPos.y, camPos.z);
            ImGui::Text("Target: %.2f, %.2f, %.2f", camTarget.x, camTarget.y, camTarget.z);
        }

        if (ImGui::CollapsingHeader("Selected Object", ImGuiTreeNodeFlags_DefaultOpen)) {
            SceneObject* active = scene->GetActiveMesh();
            if (active) {
                Mesh* mesh = static_cast<Mesh*>(active);
                glm::vec3 pos = mesh->GetPosition();
                glm::vec3 scale = mesh->GetScale();
                
                ImGui::Text("Position: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
                ImGui::Text("Scale: %.2f, %.2f, %.2f", scale.x, scale.y, scale.z);
                // Add more mesh info here
            } else {
                ImGui::Text("No object selected");
            }
        }

        if (ImGui::CollapsingHeader("Render Settings")) {
            ImGui::Checkbox("Show Vertex Points", &debugState.showVertexPoints);
            if (debugState.showVertexPoints) {
                static float pointSize = 3.0f;
                ImGui::DragFloat("Point Size", &pointSize, 0.1f, 1.0f, 10.0f);
                glPointSize(pointSize);
            }

            ImGui::Checkbox("Show Normals", &debugState.showNormals);
            ImGui::Checkbox("Show Bounding Boxes", &debugState.showBoundingBoxes);
            ImGui::Checkbox("Show Grid", &debugState.showGrid);
        }
    }
    ImGui::End();
}

void UIManager::RenderSceneHierarchy() {
    auto scene = renderer->getActiveScene();
    
    // Add object button at the top
    if (ImGui::Button("Add Object")) {
        ImGui::OpenPopup("AddObjectPopup");
    }

    if (ImGui::BeginPopup("AddObjectPopup")) {
        if (ImGui::MenuItem("Cube")) {
            Mesh* cube = MeshFactory::CreateCube(renderer->GetShader());
            cube->SetName("Cube");
            scene->AddObject(cube);
        }
        if (ImGui::MenuItem("Sphere")) {
            Mesh* sphere = MeshFactory::CreateSphere(renderer->GetShader());
            sphere->SetName("Sphere");
            scene->AddObject(sphere);
        }
        if (ImGui::MenuItem("Plane")) {
            Mesh* plane = MeshFactory::CreatePlane(renderer->GetShader());
            plane->SetName("Plane");
            scene->AddObject(plane);
        }
        if (ImGui::MenuItem("Cylinder")) {
            Mesh* cylinder = MeshFactory::CreateCylinder(renderer->GetShader());
            cylinder->SetName("Cylinder");
            scene->AddObject(cylinder);
        }
        ImGui::EndPopup();
    }

    ImGui::Separator();

    // Filter input
    static ImGuiTextFilter filter;
    filter.Draw("Filter", ImGui::GetWindowWidth() * 0.7f);

    ImGui::Separator();

    // Scene tree
    std::function<void(Mesh*, int&)> renderNode;
    renderNode = [&](Mesh* mesh, int& id) {
        if (!filter.PassFilter(mesh->GetName().c_str())) {
            return;
        }

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        
        // Add selected flag if this is the active mesh
        if (mesh == scene->GetActiveMesh()) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        // If no children, make it a leaf node
        const auto& children = mesh->GetChildren();
        if (children.empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        // Add visibility indicator
        std::string nodeLabel = mesh->IsVisible() ? "👁 " : "👁‍🗨 ";
        nodeLabel += mesh->GetName().empty() ? "Node " + std::to_string(id) : mesh->GetName();

        // Add layer indicator
        switch (mesh->GetLayer()) {
            case MeshLayer::Background: nodeLabel += " [BG]"; break;
            case MeshLayer::Environment: nodeLabel += " [ENV]"; break;
            case MeshLayer::Props: nodeLabel += " [PROP]"; break;
            case MeshLayer::Characters: nodeLabel += " [CHAR]"; break;
            case MeshLayer::UI: nodeLabel += " [UI]"; break;
            case MeshLayer::Effects: nodeLabel += " [FX]"; break;
            default: break;
        }

        bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, "%s", nodeLabel.c_str());
        
        // Handle selection
        if (ImGui::IsItemClicked()) {
            scene->SetActiveMesh(mesh);
        }

        // Context menu
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete")) {
                scene->RemoveObject(mesh);
                if (scene->GetActiveMesh() == mesh) {
                    scene->SetActiveMesh(nullptr);
                }
            }
            if (ImGui::MenuItem("Focus")) {
                glm::vec3 pos = mesh->GetPosition();
                glm::vec3 newCameraPos = pos - glm::vec3(0.0f, 0.0f, 5.0f);
                scene->SetCameraView(newCameraPos, pos, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (ImGui::MenuItem(mesh->IsVisible() ? "Hide" : "Show")) {
                mesh->SetVisible(!mesh->IsVisible());
            }
            ImGui::EndPopup();
        }

        if (nodeOpen) {
            // Render child nodes
            for (auto* child : children) {
                id++;
                renderNode(static_cast<Mesh*>(child), id);
            }
            ImGui::TreePop();
        }
    };

    // Render all root nodes
    int id = 0;
    for (auto* obj : scene->GetSceneNodes()) {
        Mesh* mesh = static_cast<Mesh*>(obj);
        if (!mesh->GetParent()) { // Only render root nodes
            int currentId = id++;
            renderNode(mesh, currentId);
        }
    }
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
