#include <glad/glad.h>
#include "Scene/Mesh.h"
#include <iostream>

Mesh::Mesh(Shader *shader):shader(shader) {
    position = glm::vec3(0.0f);
    rotationAngle = 0.0f;
    rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);  
    scale = glm::vec3(1.0f);
    UpdateModelMatrix();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Mesh::LoadMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    indexCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Define vertex attributes (position only, 3 floats per vertex)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

}

void Mesh::Translate(const glm::vec3& translation) {
    position += translation;
    UpdateModelMatrix();
}

void Mesh::Rotate(float angle, const glm::vec3& axis) {
    rotationAngle += angle;
    rotationAxis = axis;
    UpdateModelMatrix();
}

void Mesh::Scale(const glm::vec3& scaleFactor) {
    scale *= scaleFactor;
    UpdateModelMatrix();
}

void Mesh::UpdateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    modelMatrix = glm::scale(modelMatrix, scale);
}



void Mesh::Update(float deltaTime) {

}

void Mesh::Render(Shader *shader1) {
    // for (int i = 0; i < 4; ++i) {
    //     glm::vec4 row = glm::row(modelMatrix, i);
    //     std::cout << row.x << " " << row.y << " " << row.z << " " << row.w << std::endl;
    // }
    shader1->SetUniformMat4("model",modelMatrix);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);  
    glBindVertexArray(0);
}