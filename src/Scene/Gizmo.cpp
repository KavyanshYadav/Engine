#include "Scene/Gizmo.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Gizmo::Gizmo() : position(0.0f), scale(1.0f), isVisible(true) {
    InitializeGizmo();
}

Gizmo::~Gizmo() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Gizmo::InitializeGizmo() {
    // Create vertices for three axes (X, Y, Z)
    // Each axis is a line from origin extending in its direction
    vertices = {
        // X-axis (Red)
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Origin
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // X direction
        
        // Y-axis (Green)
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Origin
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Y direction
        
        // Z-axis (Blue)
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Origin
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f   // Z direction
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Gizmo::Update(float deltaTime) {
    // No update logic needed for now
}

void Gizmo::Render(Shader* shader) {
    if (!isVisible) return;

    shader->Use();

    // Calculate model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(scale));

    // Set uniforms
    shader->SetUniformMat4("model", model);

    // Enable line width
    glLineWidth(2.0f);
    
    // Draw the gizmo
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 6); // 3 axes, 2 vertices each
    glBindVertexArray(0);

    // Reset line width
    glLineWidth(1.0f);
}

void Gizmo::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Gizmo::SetScale(float newScale) {
    scale = newScale;
} 