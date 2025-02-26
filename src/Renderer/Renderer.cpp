#include <glad/glad.h>
#include "Renderer.h"
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


Renderer::Renderer(GLFWwindow* window) {
    if (!window) {
        std::cerr << "GLFW window is null!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

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
    Mesh* triangle =  new Mesh();
    Mesh* Sqaure =  new Mesh();


    Sqaure->LoadMesh(SqauareVertices,triangleIndices);

    scene->AddObject(Sqaure);
    Scenes.push_back(scene);
    this->shaderProgram = shader->CreateShaderProgram();

}

void Renderer::RenderImgui() {
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Example UI Window
    ImGui::Begin("Debug Window");
    ImGui::Text("Hello, ImGui!");
    if (ImGui::Button("Press Me")) {
        std::cout << "Button Pressed!" << std::endl;
    }
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
}

void Renderer::RenderTriangle() {
    float timeValue = glfwGetTime();
    float redValue = (sin(timeValue) / 2.0f) + 0.5f;
    shader->Use();
    shader->SetUniform4f("ourColor", redValue, 0.5f, 0.2f, 1.0f);

    for (Scene* Scene : Scenes) {
        Scene->Render();
    }
}

void Renderer::Clear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
