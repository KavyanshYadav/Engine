#include <glad/glad.h>
#include "Renderer.h"
#include "Input.h"
#include "Scene/MeshFactory.h"
#include "Scene/Material.h"
#include <iostream>


//const char* vertexShaderSource = R"(
//    #version 450 core
//    layout (location = 0) in vec3 aPos;
//
//    void main() {
//        gl_Position = vec4(aPos, 1.0);
//    }
//)";
//
//const char* fragmentShaderSource = R"(
//    #version 450 core
//    out vec4 FragColor;
//
//    void main() {
//        FragColor = vec4(1.0, 0.5, 0.2, 1.0); // Orange color
//    }
//)";
int windowWidth = 1280, windowHeight = 720;
int panelWidth = 300, panelHeight = 150;

Renderer::Renderer(Window * window):activeWindowClass(window){
    if (!window) {
        std::cerr << "GLFW window is null!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set up input system first
    input = new Input(this);
    
    // Set the window user pointer to this renderer instance
    glfwSetWindowUserPointer(window->GetGLFWWindow(), this);
    
    // Initialize input callbacks after setting user pointer
    input->InitializeInputCallbacks(window->GetGLFWWindow());

   PanelX = new int(350);
   PanelY = new int(150);
    shader = new Shader("SHADERS/vertexShader.vert", "SHADERS/fragmentShader.frag");
    
    // Enable depth and stencil testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    
    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    std::vector<float> triangleVertices = {
        // Positions         // Colors
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Red
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Green
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Blue
    };

    std::vector<float> square2Vertices = {
        // Positions         // Colors
         0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // Yellow
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,  // Cyan
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,  // Magenta
         0.0f, -0.0f, 0.7f, 1.0f, 1.0f, 1.0f   // White
    };

    std::vector<unsigned int> triangleIndices = {
        0, 1, 2  // Triangle
    };

    std::vector<unsigned int> squareIndices = {
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    Scene* scene = new Scene();
    
    // Create a cube with enhanced metal material
    Mesh* cube = MeshFactory::CreateCube(shader);
    Material* metalMaterial = Material::CreateMetal();
    metalMaterial->SetAlbedo(glm::vec3(0.98f, 0.98f, 0.98f));  // More reflective base color
    metalMaterial->SetMetallic(1.0f);                           // Fully metallic
    metalMaterial->SetRoughness(0.1f);                          // Very smooth surface
    cube->AddMaterial(metalMaterial);
    cube->SetPosition(glm::vec3(-1.5f, 0.0f, 0.0f));
    scene->AddObject(cube);
    
    // Create a cylinder with wood material
    Mesh* cylinder = MeshFactory::CreateCylinder(shader, 0.5f, 1.0f, 32);
    Material* woodMaterial = Material::CreateWood();
    cylinder->AddMaterial(woodMaterial);
    cylinder->SetPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    scene->AddObject(cylinder);
    
    Scenes.push_back(scene);
    this->shaderProgram = shader->CreateShaderProgram();

    // Initialize axis lines
    InitializeAxisLines();
}


void Renderer::ReloadShader( ){
    glDeleteProgram(shaderProgram);
    delete shader;
    shader = new Shader("SHADERS/vertexShader.vert", "SHADERS/fragmentShader.frag");
    shaderProgram = shader->CreateShaderProgram();

}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Cleanup axis lines
    CleanupAxisLines();
}
Scene* Renderer::getActiveScene()
{
    return Scenes.front();
}


void Renderer::RenderTriangle() {
    // We no longer need to set the ourColor uniform since we use vertex colors
    shader->Use();
    
    for (Scene* Scene : Scenes) {
        Scene->Render(shader);
    }
}

void Renderer::Clear() {
    // Reset statistics for this frame
    drawCalls = 0;
    triangleCount = 0;
    vertexCount = 0;
    textureCount = 0;
    shaderSwitches = 0;

    // First clear the entire window with a darker color
    glDisable(GL_SCISSOR_TEST);
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);  // Darker grey for window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // Then set up and clear the viewport with the background color
    glViewport(*PanelX, *PanelY, activeWindowClass->getWindowSize().x - panelWidth, activeWindowClass->getWindowSize().y - panelHeight);
    glEnable(GL_SCISSOR_TEST);
    glScissor(*PanelX, *PanelY, activeWindowClass->getWindowSize().x - panelWidth, activeWindowClass->getWindowSize().y - panelHeight);
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);  // Use custom background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // First render the scene meshes
    for (Scene* scene : Scenes) {
        // Update statistics before rendering
        for (const auto& obj : scene->GetSceneNodes()) {
            if (auto mesh = dynamic_cast<Mesh*>(obj)) {
                const auto& stats = mesh->GetStats();
                triangleCount += stats.triangleCount;
                vertexCount += stats.vertexCount;
                drawCalls++;
            }
        }
        scene->Render(shader);
    }

    // Count active textures
    GLint activeTextures = 0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTextures);
    textureCount = activeTextures;

    // Then render the axis lines on top
    RenderAxisLines();
}

void Renderer::InitializeAxisLines() {
    if (axisLines.initialized) return;

    // Create and compile the axis shader
    axisLines.shader = new Shader("SHADERS/axis.vert", "SHADERS/axis.frag");
    axisLines.shader->CreateShaderProgram();  // Create the shader program

    // Create vertex data for infinite axis lines
    // Each line has two vertices with positions and colors
    float axisVertices[] = {
        // Positions          // Colors
        -1000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // X axis (red)
         1000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        
        0.0f, -1000.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Y axis (green)
        0.0f,  1000.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        
        0.0f, 0.0f, -1000.0f, 0.0f, 0.0f, 1.0f,  // Z axis (blue)
        0.0f, 0.0f,  1000.0f, 0.0f, 0.0f, 1.0f
    };

    // Create and bind VAO and VBO
    glGenVertexArrays(1, &axisLines.VAO);
    glGenBuffers(1, &axisLines.VBO);

    glBindVertexArray(axisLines.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisLines.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    axisLines.initialized = true;
}

void Renderer::RenderAxisLines() {
    if (!axisLines.initialized) {
        InitializeAxisLines();
    }

    // Enable line width (if supported by your graphics driver)
    glLineWidth(2.0f);

    // Use the axis shader
    axisLines.shader->Use();

    // Get matrices from scene
    Scene* scene = getActiveScene();
    glm::mat4 view = scene->GetViewMatrix();
    glm::mat4 projection = scene->GetProjectionMatrix();

    // Set shader uniforms
    axisLines.shader->SetUniformMat4("view", view);
    axisLines.shader->SetUniformMat4("projection", projection);

    // Draw the axis lines
    glBindVertexArray(axisLines.VAO);
    glDrawArrays(GL_LINES, 0, 6); // 3 lines, 2 vertices each

    // Reset OpenGL state
    glLineWidth(1.0f);
}

void Renderer::CleanupAxisLines() {
    if (axisLines.initialized) {
        glDeleteVertexArrays(1, &axisLines.VAO);
        glDeleteBuffers(1, &axisLines.VBO);
        delete axisLines.shader;
        axisLines.initialized = false;
    }
}
