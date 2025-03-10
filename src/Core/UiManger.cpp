#include "UiManger.h"
#include <functional>
#include <Windows.h>
#include <psapi.h>
#include <pdh.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "imgui_internal.h"
#include "Logger.h"
#include <filesystem>

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

    // 5. Camera Controls (overlay on viewport, bottom left)
    float camControlWidth = 250.0f;  // Increased width for sliders
    float camControlHeight = 180.0f;  // Increased height for additional controls
    ImGui::SetNextWindowPos(ImVec2(
        propertyPanelWidth + 10,  // Just after the properties panel
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

    // Render system info window if enabled
    RenderSystemInfoWindow();

    // Update performance metrics
    UpdatePerformanceMetrics(ImGui::GetIO().DeltaTime);

    // Add performance window rendering
    RenderPerformanceWindow();
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

    // Light Properties
    if (ImGui::CollapsingHeader("Lights")) {
        auto lights = scene->GetLights();
        for (size_t i = 0; i < lights.size(); i++) {
            Light* light = lights[i];
            if (ImGui::TreeNode(("Light " + std::to_string(i)).c_str())) {
                // Position
                glm::vec3 pos = light->GetPosition();
                float position[3] = { pos.x, pos.y, pos.z };
                if (ImGui::DragFloat3("Position", position, 0.1f)) {
                    light->SetPosition(glm::vec3(position[0], position[1], position[2]));
                }

                // Color
                glm::vec3 col = light->GetColor();
                float color[3] = { col.x, col.y, col.z };
                if (ImGui::ColorEdit3("Color", color)) {
                    light->SetColor(glm::vec3(color[0], color[1], color[2]));
                }

                // Intensity
                float intensity = light->GetIntensity();
                if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f)) {
                    light->SetIntensity(intensity);
                }

                if (ImGui::Button("Remove Light")) {
                    scene->RemoveLight(light);
                }
                ImGui::TreePop();
            }
        }

        if (ImGui::Button("Add Light")) {
            Light* newLight = new Light(
                glm::vec3(0.0f, 5.0f, 0.0f),  // position
                glm::vec3(1.0f, 1.0f, 1.0f),  // color
                1.0f                          // intensity
            );
            scene->AddLight(newLight);
        }
    }

    ImGui::End();
}

void UIManager::RenderTimelinePanel() {
    int width = ImGui::GetIO().DisplaySize.x;
    int height = ImGui::GetIO().DisplaySize.y;
    
    // File System Panel (left side of the bottom panel)
    ImGui::SetNextWindowPos(ImVec2(propertyPanelWidth, height - timelinePanelHeight));
    ImGui::SetNextWindowSize(ImVec2(fileSystemPanelWidth, timelinePanelHeight));
    RenderFileSystemPanel();

    // Timeline Panel (right side of the bottom panel)
    ImGui::SetNextWindowPos(ImVec2(propertyPanelWidth + fileSystemPanelWidth, height - timelinePanelHeight));
    ImGui::SetNextWindowSize(ImVec2(width - propertyPanelWidth - fileSystemPanelWidth, timelinePanelHeight));
    ImGui::Begin("Timeline", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse);

    RenderTimelineTabs();

    ImGui::End();
}

void UIManager::RenderFileSystemPanel() {
    ImGui::Begin("File System", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse);

    // Navigation bar
    if (ImGui::Button("◄")) {
        auto parent = std::filesystem::path(fileSystemState.currentPath).parent_path();
        if (!parent.empty()) {
            NavigateToDirectory(parent.string());
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("⟳")) {
        RefreshDirectoryContents();
    }
    ImGui::SameLine();
    if (ImGui::Button("↑")) {
        NavigateToDirectory(std::filesystem::path(fileSystemState.currentPath).parent_path().string());
    }
    ImGui::SameLine();
    
    // Current path display and navigation
    char pathBuffer[1024];
    strcpy_s(pathBuffer, fileSystemState.currentPath.c_str());
    if (ImGui::InputText("##Path", pathBuffer, sizeof(pathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        NavigateToDirectory(pathBuffer);
    }

    // Search filter
    ImGui::Separator();
    fileSystemState.filter.Draw("Filter", ImGui::GetWindowWidth() * 0.7f);
    ImGui::SameLine();
    ImGui::Checkbox("Show Hidden", &fileSystemState.showHiddenFiles);

    // Favorites/Quick Access
    if (ImGui::BeginChild("Favorites", ImVec2(0, 80), true)) {
        for (const auto& favorite : fileSystemState.favoriteDirectories) {
            if (ImGui::Button(std::filesystem::path(favorite).filename().string().c_str())) {
                NavigateToDirectory(favorite);
            }
            ImGui::SameLine();
        }
    }
    ImGui::EndChild();

    // File/Directory list
    ImGui::BeginChild("Files", ImVec2(0, 0), true);
    
    // Handle drag and drop
    HandleFileDrop();

    // Display directories first, then files
    for (const auto& entry : fileSystemState.currentEntries) {
        if (!fileSystemState.showHiddenFiles && entry.path().filename().string()[0] == '.') {
            continue;
        }

        if (!fileSystemState.filter.PassFilter(entry.path().filename().string().c_str())) {
            continue;
        }

        if (std::filesystem::is_directory(entry)) {
            RenderFileIcon(entry);
        }
    }

    for (const auto& entry : fileSystemState.currentEntries) {
        if (!fileSystemState.showHiddenFiles && entry.path().filename().string()[0] == '.') {
            continue;
        }

        if (!fileSystemState.filter.PassFilter(entry.path().filename().string().c_str())) {
            continue;
        }

        if (!std::filesystem::is_directory(entry)) {
            RenderFileIcon(entry);
        }
    }

    ImGui::EndChild();
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
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Camera Controls", nullptr, cameraFlags);
    
    Scene* scene = renderer->getActiveScene();
    
    if (ImGui::Button("Reset Camera")) {
        // Reset to isometric view (Blender-style)
        scene->SetCameraView(
            glm::vec3(7.3589f, 6.9258f, 4.9583f),  // Isometric position
            glm::vec3(0.0f, 0.0f, 0.0f),           // Looking at origin
            glm::vec3(0.0f, 1.0f, 0.0f)            // Up vector
        );
    }

    // Camera position sliders
    glm::vec3 camPos = scene->GetCameraPosition();
    bool positionChanged = false;
    
    // X position
    if (ImGui::SliderFloat("X", &camPos.x, -10.0f, 10.0f)) {
        positionChanged = true;
    }
    
    // Y position
    if (ImGui::SliderFloat("Y", &camPos.y, -10.0f, 10.0f)) {
        positionChanged = true;
    }
    
    // Z position
    if (ImGui::SliderFloat("Z", &camPos.z, -10.0f, 10.0f)) {
        positionChanged = true;
    }

    if (positionChanged) {
        scene->SetCameraView(camPos, 
            scene->GetCameraTarget(),
            glm::vec3(0.0f, 1.0f, 0.0f));
    }

    ImGui::Separator();

    // View buttons
    float buttonWidth = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2) / 3;
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));
    if (ImGui::Button("Top", ImVec2(buttonWidth, 0))) {
        scene->SetCameraView(
            glm::vec3(0.0f, 10.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        );
    }
    ImGui::SameLine();
    if (ImGui::Button("Front", ImVec2(buttonWidth, 0))) {
        scene->SetCameraView(
            glm::vec3(0.0f, 0.0f, 10.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }
    ImGui::SameLine();
    if (ImGui::Button("Side", ImVec2(buttonWidth, 0))) {
        scene->SetCameraView(
            glm::vec3(10.0f, 0.0f, 0.0f),
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

    // System Info Button
    if (ImGui::Button("System Info")) {
        uiState.showSystemInfo = true;
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
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();

    if (ImGui::Button("Performance")) {
        performanceState.showPerformanceWindow = true;
    }

    ImGui::End();
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
                float gridSize = debugState.gridSize;
                int gridLines = debugState.gridLines;
                if (ImGui::DragFloat("Grid Size", &gridSize, 0.1f, 0.1f, 10.0f)) {
                    debugState.gridSize = gridSize;
                }
                if (ImGui::DragInt("Grid Lines", &gridLines, 1, 2, 100)) {
                    debugState.gridLines = gridLines;
                }
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

void UIManager::RenderSystemInfoWindow() {
    if (!uiState.showSystemInfo) return;

    // Make sure ConfigManager is initialized when we first open the window
    static bool configInitialized = false;
    if (!configInitialized) {
        ConfigManager::GetInstance().Initialize();
        configInitialized = true;
    }

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("System Information", &uiState.showSystemInfo)) {
        auto& configManager = ConfigManager::GetInstance();

        // CPU Information
        if (ImGui::CollapsingHeader("CPU Information", ImGuiTreeNodeFlags_DefaultOpen)) {
            std::string vendor = configManager.GetConfigValue("System.CPU.Vendor");
            std::string brand = configManager.GetConfigValue("System.CPU.Brand");
            std::string arch = configManager.GetConfigValue("System.CPU.Architecture");

            ImGui::Text("Vendor: %s", vendor.empty() ? "Unknown" : vendor.c_str());
            ImGui::Text("Brand: %s", brand.empty() ? "Unknown" : brand.c_str());
            
            // Use try-catch for integer conversions
            try {
                ImGui::Text("Cores: %d", configManager.GetConfigValueInt("System.CPU.Cores"));
                ImGui::Text("Threads: %d", configManager.GetConfigValueInt("System.CPU.Threads"));
            } catch (...) {
                ImGui::Text("Cores: Unknown");
                ImGui::Text("Threads: Unknown");
            }
            
            ImGui::Text("Architecture: %s", arch.empty() ? "Unknown" : arch.c_str());
            
            // CPU Features
            std::string features = configManager.GetConfigValue("System.CPU.Features");
            if (!features.empty()) {
                ImGui::Text("Features:");
                std::stringstream ss(features);
                std::string feature;
                ImGui::Indent();
                while (std::getline(ss, feature, ';')) {
                    if (!feature.empty()) {
                        ImGui::BulletText("%s", feature.c_str());
                    }
                }
                ImGui::Unindent();
            }
        }

        // GPU Information
        if (ImGui::CollapsingHeader("GPU Information", ImGuiTreeNodeFlags_DefaultOpen)) {
            std::string vendor = configManager.GetConfigValue("System.GPU.Vendor");
            std::string renderer = configManager.GetConfigValue("System.GPU.Renderer");
            std::string version = configManager.GetConfigValue("System.GPU.Version");
            std::string glslVersion = configManager.GetConfigValue("System.GPU.GLSLVersion");

            ImGui::Text("Vendor: %s", vendor.empty() ? "Unknown" : vendor.c_str());
            ImGui::Text("Renderer: %s", renderer.empty() ? "Unknown" : renderer.c_str());
            ImGui::Text("Version: %s", version.empty() ? "Unknown" : version.c_str());
            ImGui::Text("GLSL Version: %s", glslVersion.empty() ? "Unknown" : glslVersion.c_str());
            
            // GPU Extensions
            std::string extensions = configManager.GetConfigValue("System.GPU.Extensions");
            if (!extensions.empty() && ImGui::TreeNode("Extensions")) {
                std::stringstream ss(extensions);
                std::string ext;
                while (std::getline(ss, ext, ';')) {
                    if (!ext.empty()) {
                        ImGui::BulletText("%s", ext.c_str());
                    }
                }
            ImGui::TreePop();
        }
        }

        // Memory Information
        if (ImGui::CollapsingHeader("Memory Information", ImGuiTreeNodeFlags_DefaultOpen)) {
            try {
                float totalPhysMB = std::stof(configManager.GetConfigValue("System.Memory.TotalPhysical"));
                float availPhysMB = std::stof(configManager.GetConfigValue("System.Memory.AvailablePhysical"));
                float totalVirtMB = std::stof(configManager.GetConfigValue("System.Memory.TotalVirtual"));
                float availVirtMB = std::stof(configManager.GetConfigValue("System.Memory.AvailableVirtual"));

                ImGui::Text("Physical Memory:");
                ImGui::Indent();
                ImGui::Text("Total: %.2f GB", totalPhysMB / 1024.0f);
                ImGui::Text("Available: %.2f GB", availPhysMB / 1024.0f);
                ImGui::Text("Used: %.2f GB", (totalPhysMB - availPhysMB) / 1024.0f);
                ImGui::Unindent();

                ImGui::Text("Virtual Memory:");
                ImGui::Indent();
                ImGui::Text("Total: %.2f GB", totalVirtMB / 1024.0f);
                ImGui::Text("Available: %.2f GB", availVirtMB / 1024.0f);
                ImGui::Text("Used: %.2f GB", (totalVirtMB - availVirtMB) / 1024.0f);
                ImGui::Unindent();
            } catch (...) {
                ImGui::Text("Memory information unavailable");
            }
        }

        // Operating System Information
        if (ImGui::CollapsingHeader("Operating System", ImGuiTreeNodeFlags_DefaultOpen)) {
            std::string os = configManager.GetConfigValue("System.OS");
            ImGui::Text("%s", os.empty() ? "Unknown" : os.c_str());
        }

        // System Requirements Status
        if (ImGui::CollapsingHeader("System Requirements")) {
            bool meetsMin = configManager.MeetsMinimumRequirements();
            bool meetsRec = configManager.MeetsRecommendedRequirements();

            ImGui::TextColored(
                meetsMin ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                "Minimum Requirements: %s", meetsMin ? "Met" : "Not Met"
            );

            ImGui::TextColored(
                meetsRec ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
                "Recommended Requirements: %s", meetsRec ? "Met" : "Not Met"
            );
        }
    }
    ImGui::End();
}

void UIManager::RenderPerformanceWindow() {
    if (!performanceState.showPerformanceWindow) return;

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Performance Monitor", &performanceState.showPerformanceWindow)) {
        auto& metrics = performanceState.metrics;

        // Control bar
        if (ImGui::Button(performanceState.pauseCollection ? "Resume" : "Pause")) {
            performanceState.pauseCollection = !performanceState.pauseCollection;
        }
        ImGui::SameLine();
        ImGui::Checkbox("CPU Graph", &performanceState.showCPUGraph);
        ImGui::SameLine();
        ImGui::Checkbox("GPU Graph", &performanceState.showGPUGraph);
        ImGui::SameLine();
        ImGui::Checkbox("Memory Graph", &performanceState.showMemoryGraph);
        ImGui::SameLine();
        ImGui::Checkbox("FPS Graph", &performanceState.showFPSGraph);

        ImGui::Separator();

        // Main metrics display
        if (ImGui::BeginTabBar("PerformanceMetrics")) {
            if (ImGui::BeginTabItem("Overview")) {
                ImGui::Columns(2);

                // Left column - CPU & GPU
                ImGui::Text("CPU Usage: %.1f%%", metrics.cpu.usagePercent);
                if (performanceState.showCPUGraph) {
                    RenderPerformanceGraph("##CPU", metrics.cpu.usageHistory, 0.0f, 100.0f, 
                                         ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                }

                ImGui::Text("GPU Usage: %.1f%%", metrics.gpu.usagePercent);
                if (performanceState.showGPUGraph) {
                    RenderPerformanceGraph("##GPU", metrics.gpu.usageHistory, 0.0f, 100.0f,
                                         ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                }

                ImGui::NextColumn();

                // Right column - Memory & FPS
                ImGui::Text("Memory Usage: %.1f / %.1f GB", 
                          metrics.memory.usedPhysicalMB / 1024.0f,
                          metrics.memory.totalPhysicalMB / 1024.0f);
                if (performanceState.showMemoryGraph) {
                    RenderPerformanceGraph("##Memory", metrics.memory.physicalHistory, 
                                         0.0f, metrics.memory.totalPhysicalMB,
                                         ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                }

                ImGui::Text("FPS: %.1f (%.2f ms)", metrics.rendering.fps, metrics.rendering.frameTime);
                if (performanceState.showFPSGraph) {
                    RenderPerformanceGraph("##FPS", metrics.rendering.fpsHistory, 
                                         0.0f, 200.0f,
                                         ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
                }

                ImGui::Columns(1);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("CPU Details")) {
                ImGui::Text("CPU Usage: %.1f%%", metrics.cpu.usagePercent);
                ImGui::Text("Active Threads: %d", metrics.cpu.numThreadsActive);
                ImGui::Text("Clock Speed: %.2f GHz", metrics.cpu.clockSpeed);
                ImGui::Text("Temperature: %.1f°C", metrics.cpu.temperature);
                
                if (ImGui::CollapsingHeader("Usage History", ImGuiTreeNodeFlags_DefaultOpen)) {
                    RenderPerformanceGraph("Usage %", metrics.cpu.usageHistory, 
                                         0.0f, 100.0f,
                                         ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                }
                
                if (ImGui::CollapsingHeader("Temperature History")) {
                    RenderPerformanceGraph("Temperature °C", metrics.cpu.temperatureHistory,
                                         0.0f, 100.0f,
                                         ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("GPU Details")) {
                ImGui::Text("GPU Usage: %.1f%%", metrics.gpu.usagePercent);
                ImGui::Text("VRAM Usage: %.1f / %.1f MB", metrics.gpu.vramUsageMB, metrics.gpu.vramTotalMB);
                ImGui::Text("Clock Speed: %.0f MHz", metrics.gpu.clockSpeed);
                ImGui::Text("Temperature: %.1f°C", metrics.gpu.temperature);
                
                if (ImGui::CollapsingHeader("Usage History", ImGuiTreeNodeFlags_DefaultOpen)) {
                    RenderPerformanceGraph("Usage %", metrics.gpu.usageHistory,
                                         0.0f, 100.0f,
                                         ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                }
                
                if (ImGui::CollapsingHeader("VRAM History")) {
                    RenderPerformanceGraph("VRAM MB", metrics.gpu.vramHistory,
                                         0.0f, metrics.gpu.vramTotalMB,
                                         ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Memory Details")) {
                ImGui::Text("Physical Memory: %.1f / %.1f GB (%.1f%%)", 
                          metrics.memory.usedPhysicalMB / 1024.0f,
                          metrics.memory.totalPhysicalMB / 1024.0f,
                          (metrics.memory.usedPhysicalMB / metrics.memory.totalPhysicalMB) * 100.0f);
                
                ImGui::Text("Virtual Memory: %.1f / %.1f GB (%.1f%%)",
                          metrics.memory.usedVirtualMB / 1024.0f,
                          metrics.memory.totalVirtualMB / 1024.0f,
                          (metrics.memory.usedVirtualMB / metrics.memory.totalVirtualMB) * 100.0f);
                
                ImGui::Text("Page File Usage: %.1f%%", metrics.memory.pageFileUsage * 100.0f);
                
                if (ImGui::CollapsingHeader("Physical Memory History", ImGuiTreeNodeFlags_DefaultOpen)) {
                    RenderPerformanceGraph("Physical MB", metrics.memory.physicalHistory,
                                         0.0f, metrics.memory.totalPhysicalMB,
                                         ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                }
                
                if (ImGui::CollapsingHeader("Virtual Memory History")) {
                    RenderPerformanceGraph("Virtual MB", metrics.memory.virtualHistory,
                                         0.0f, metrics.memory.totalVirtualMB,
                                         ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Rendering Stats")) {
                ImGui::Columns(2);
                
                ImGui::Text("FPS: %.1f", metrics.rendering.fps);
                ImGui::Text("Frame Time: %.2f ms", metrics.rendering.frameTime);
                ImGui::Text("GPU Time: %.2f ms", metrics.rendering.gpuFrameTime);
                ImGui::Text("CPU Time: %.2f ms", metrics.rendering.cpuFrameTime);
                
                ImGui::NextColumn();
                
                ImGui::Text("Draw Calls: %d", metrics.statistics.drawCalls);
                ImGui::Text("Triangles: %d", metrics.statistics.triangles);
                ImGui::Text("Vertices: %d", metrics.statistics.vertices);
                ImGui::Text("Textures: %d", metrics.statistics.textures);
                ImGui::Text("Shader Switches: %d", metrics.statistics.shaderSwitches);
                ImGui::Text("Batched Draw Calls: %.0f", metrics.statistics.batchedDrawCalls);
                ImGui::Text("Culled Objects: %.0f", metrics.statistics.culledObjects);
                
                ImGui::Columns(1);
                
                if (ImGui::CollapsingHeader("Frame Time History", ImGuiTreeNodeFlags_DefaultOpen)) {
                    RenderPerformanceGraph("Frame Time ms", metrics.rendering.frameTimeHistory,
                                         0.0f, 33.3f,
                                         ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
                }
                
                if (ImGui::CollapsingHeader("FPS History")) {
                    RenderPerformanceGraph("FPS", metrics.rendering.fpsHistory,
                                         0.0f, 200.0f,
                                         ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                }
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void UIManager::UpdatePerformanceMetrics(float deltaTime) {
    if (performanceState.pauseCollection) return;

    performanceState.timeSinceLastUpdate += deltaTime;
    if (performanceState.timeSinceLastUpdate >= performanceState.updateInterval) {
        CollectPerformanceMetrics();
        performanceState.timeSinceLastUpdate = 0.0f;
    }

    // Update FPS and frame time every frame
    auto& metrics = performanceState.metrics;
    metrics.rendering.fps = ImGui::GetIO().Framerate;
    metrics.rendering.frameTime = 1000.0f / metrics.rendering.fps;

    // Keep history size constant
    const size_t maxHistory = performanceState.historySize;
    auto& fpsHistory = metrics.rendering.fpsHistory;
    auto& frameTimeHistory = metrics.rendering.frameTimeHistory;

    fpsHistory.push_back(metrics.rendering.fps);
    frameTimeHistory.push_back(metrics.rendering.frameTime);

    if (fpsHistory.size() > maxHistory) fpsHistory.erase(fpsHistory.begin());
    if (frameTimeHistory.size() > maxHistory) frameTimeHistory.erase(frameTimeHistory.begin());
}

void UIManager::CollectPerformanceMetrics() {
    auto& metrics = performanceState.metrics;
    
    // Get CPU metrics
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    
    static ULARGE_INTEGER lastIdleTime = {0};
    static ULARGE_INTEGER lastKernelTime = {0};
    static ULARGE_INTEGER lastUserTime = {0};
    
    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;
    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;
    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;

    if (lastIdleTime.QuadPart != 0) {
        ULONGLONG idleDiff = idle.QuadPart - lastIdleTime.QuadPart;
        ULONGLONG kernelDiff = kernel.QuadPart - lastKernelTime.QuadPart;
        ULONGLONG userDiff = user.QuadPart - lastUserTime.QuadPart;
        ULONGLONG totalDiff = kernelDiff + userDiff;
        
        if (totalDiff > 0) {
            metrics.cpu.usagePercent = ((totalDiff - idleDiff) * 100.0f) / totalDiff;
            metrics.cpu.usageHistory.push_back(metrics.cpu.usagePercent);
            if (metrics.cpu.usageHistory.size() > performanceState.historySize) {
                metrics.cpu.usageHistory.erase(metrics.cpu.usageHistory.begin());
            }
        }
    }

    lastIdleTime = idle;
    lastKernelTime = kernel;
    lastUserTime = user;

    // Get memory metrics
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    metrics.memory.totalPhysicalMB = memInfo.ullTotalPhys / (1024.0f * 1024.0f);
    metrics.memory.usedPhysicalMB = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024.0f * 1024.0f);
    metrics.memory.totalVirtualMB = memInfo.ullTotalVirtual / (1024.0f * 1024.0f);
    metrics.memory.usedVirtualMB = (memInfo.ullTotalVirtual - memInfo.ullAvailVirtual) / (1024.0f * 1024.0f);
    metrics.memory.pageFileUsage = memInfo.dwMemoryLoad / 100.0f;

    metrics.memory.physicalHistory.push_back(metrics.memory.usedPhysicalMB);
    metrics.memory.virtualHistory.push_back(metrics.memory.usedVirtualMB);

    if (metrics.memory.physicalHistory.size() > performanceState.historySize) {
        metrics.memory.physicalHistory.erase(metrics.memory.physicalHistory.begin());
    }
    if (metrics.memory.virtualHistory.size() > performanceState.historySize) {
        metrics.memory.virtualHistory.erase(metrics.memory.virtualHistory.begin());
    }

    // Get GPU metrics from OpenGL
    GLint totalMemKb = 0;
    GLint currentMemKb = 0;
    glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemKb);
    glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &currentMemKb);

    metrics.gpu.vramTotalMB = totalMemKb / 1024.0f;
    metrics.gpu.vramUsageMB = (totalMemKb - currentMemKb) / 1024.0f;
    metrics.gpu.usagePercent = (metrics.gpu.vramUsageMB / metrics.gpu.vramTotalMB) * 100.0f;

    metrics.gpu.usageHistory.push_back(metrics.gpu.usagePercent);
    metrics.gpu.vramHistory.push_back(metrics.gpu.vramUsageMB);

    if (metrics.gpu.usageHistory.size() > performanceState.historySize) {
        metrics.gpu.usageHistory.erase(metrics.gpu.usageHistory.begin());
    }
    if (metrics.gpu.vramHistory.size() > performanceState.historySize) {
        metrics.gpu.vramHistory.erase(metrics.gpu.vramHistory.begin());
    }

    // Update rendering statistics
    auto scene = renderer->getActiveScene();
    metrics.statistics.drawCalls = renderer->GetDrawCalls();
    metrics.statistics.triangles = renderer->GetTriangleCount();
    metrics.statistics.vertices = renderer->GetVertexCount();
    metrics.statistics.textures = renderer->GetTextureCount();
    metrics.statistics.shaderSwitches = renderer->GetShaderSwitches();
}

void UIManager::RenderPerformanceGraph(const char* label, const std::vector<float>& data, 
                                     float minScale, float maxScale, const ImVec4& color) {
    if (data.empty()) return;

    ImGui::PlotLines(label, data.data(), static_cast<int>(data.size()), 0, nullptr, 
                     minScale, maxScale, ImVec2(ImGui::GetContentRegionAvail().x, 60));
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

void UIManager::RefreshDirectoryContents() {
    fileSystemState.currentEntries.clear();
    try {
        for (const auto& entry : std::filesystem::directory_iterator(fileSystemState.currentPath)) {
            fileSystemState.currentEntries.push_back(entry);
        }
        
        // Sort entries: directories first, then files, both alphabetically
        std::sort(fileSystemState.currentEntries.begin(), fileSystemState.currentEntries.end(),
            [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
                bool aIsDir = std::filesystem::is_directory(a);
                bool bIsDir = std::filesystem::is_directory(b);
                if (aIsDir != bIsDir) return aIsDir > bIsDir;
                return a.path().filename() < b.path().filename();
            });
    }
    catch (const std::exception& e) {
        Logger::GetInstance().Critical("Failed to read directory: " + std::string(e.what()));
    }
}

void UIManager::RenderFileIcon(const std::filesystem::directory_entry& entry) {
    const float iconSize = 16.0f;
    const float spacing = 4.0f;
    bool isDirectory = std::filesystem::is_directory(entry);
    std::string filename = entry.path().filename().string();
    std::string icon = GetFileIcon(entry.path());
    
    ImGui::BeginGroup();
    
    // Icon and filename in a selectable
    bool isSelected = fileSystemState.selectedFile == entry.path().string();
    if (ImGui::Selectable((icon + " " + filename).c_str(), isSelected, 
        ImGuiSelectableFlags_AllowDoubleClick, ImVec2(ImGui::GetWindowWidth() - 20, 0))) {
        
        fileSystemState.selectedFile = entry.path().string();
        
        if (ImGui::IsMouseDoubleClicked(0)) {
            if (isDirectory) {
                NavigateToDirectory(entry.path().string());
            }
            else {
                // Handle file opening based on extension
                std::string ext = entry.path().extension().string();
                if (ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".glb") {
                    // TODO: Import 3D model
                }
                else if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp") {
                    // TODO: Import texture
                }
                // Add more file type handlers as needed
            }
        }
    }
    
    // Context menu
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Add to Favorites")) {
            fileSystemState.favoriteDirectories.push_back(entry.path().string());
        }
        if (ImGui::MenuItem("Copy Path")) {
            ImGui::SetClipboardText(entry.path().string().c_str());
        }
        if (ImGui::MenuItem("Delete")) {
            try {
                std::filesystem::remove(entry.path());
                RefreshDirectoryContents();
            }
            catch (const std::exception& e) {
                Logger::GetInstance().Critical("Failed to delete file: " + std::string(e.what()));
            }
        }
        ImGui::EndPopup();
    }
    
    ImGui::EndGroup();
}

std::string UIManager::GetFileIcon(const std::filesystem::path& path) {
    if (std::filesystem::is_directory(path)) {
        return "📁";
    }
    
    std::string ext = path.extension().string();
    if (ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".glb") {
        return "📦";  // 3D model
    }
    else if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp") {
        return "🖼";  // Image
    }
    else if (ext == ".txt" || ext == ".md") {
        return "📄";  // Text file
    }
    else if (ext == ".shader" || ext == ".vert" || ext == ".frag") {
        return "⚡";  // Shader file
    }
    else if (ext == ".cpp" || ext == ".h") {
        return "💻";  // Source code
    }
    return "📄";  // Default file icon
}

void UIManager::NavigateToDirectory(const std::string& path) {
    try {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            fileSystemState.currentPath = path;
            RefreshDirectoryContents();
        }
    }
    catch (const std::exception& e) {
        Logger::GetInstance().Critical("Failed to navigate to directory: " + std::string(e.what()));
    }
}

void UIManager::HandleFileDrop() {
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH")) {
            std::string path((const char*)payload->Data);
            if (std::filesystem::is_directory(path)) {
                NavigateToDirectory(path);
            }
            else {
                // Handle dropped file based on extension
                std::string ext = std::filesystem::path(path).extension().string();
                if (ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".glb") {
                    // TODO: Import 3D model
                }
                else if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp") {
                    // TODO: Import texture
                }
                // Add more file type handlers as needed
            }
        }
        ImGui::EndDragDropTarget();
    }
}
