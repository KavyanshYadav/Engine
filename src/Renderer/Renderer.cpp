#include <glad/glad.h>
#include "Renderer.h"
#include "Input.h"
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
    glEnable(GL_DEPTH_TEST);

    std::vector<float> triangleVertices = {
        0.0f,  0.5f, 0.0f, 
       -0.5f, -0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f   
   };
   std::vector<float> SqauareVertices = {
    0.0f,  0.5f, 0.0f,  
   -0.5f, -0.5f, 0.0f,  
    0.5f, -0.5f, 0.0f  , 
    0.0f, -0.0f, 0.7f   

};
std::vector<unsigned int> triangleIndices = {
    0, 1, 3,  // first Triangle
    1, 2, 3  
};

    Scene* scene = new Scene();
    Mesh* Sqaure =  new Mesh(shader);
    Mesh* Sqaure2 =  new Mesh(shader);

    Sqaure2->LoadMesh(triangleVertices,triangleIndices);
    Sqaure->LoadMesh(triangleVertices,triangleIndices);
    scene->AddObject(Sqaure);
    scene->AddObject(Sqaure2);
    Scenes.push_back(scene);
    Sqaure->Translate(glm::vec3(0.1f,0.0f,0.0f));
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
    float timeValue = glfwGetTime();
    float redValue = (sin(timeValue) / 2.0f) + 0.5f;
    shader->SetUniform4f("ourColor", redValue, 0.5f, 0.2f, 1.0f);
    shader->Use();
    
    for (Scene* Scene : Scenes) {
        Scene->Render(shader);
        
    }
}

void Renderer::Clear() {
    // std::cout << activeWindowClass->getWindowSize().x <<std::endl;
    
    // First clear the entire window with a darker color
    glDisable(GL_SCISSOR_TEST);
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);  // Darker grey for window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Then set up and clear the viewport with a different color
    glViewport(*PanelX, *PanelY, activeWindowClass->getWindowSize().x - panelWidth, activeWindowClass->getWindowSize().y - panelHeight);
    glEnable(GL_SCISSOR_TEST);
    glScissor(*PanelX, *PanelY, activeWindowClass->getWindowSize().x - panelWidth, activeWindowClass->getWindowSize().y - panelHeight);
    glClearColor(0.283f, 0.283f, 0.283f, 1.0f);  // Blender-like grey for viewport
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the axis lines after clearing
    RenderAxisLines();
}

void Renderer::InitializeAxisLines() {
    if (axisLines.initialized) return;

    // Create and compile the axis shader
    axisLines.shader = new Shader("SHADERS/axis.vert", "SHADERS/axis.frag");

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


    // Set view and projection matrices
    Scene* scene = getActiveScene();

    // Draw the axis lines
    glBindVertexArray(axisLines.VAO);
    glDrawArrays(GL_LINES, 0, 6); // 3 lines, 2 vertices each

    // Reset line width
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
