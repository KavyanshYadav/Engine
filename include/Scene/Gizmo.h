#pragma once
#include "SceneObject.h"
#include "../Shader.h"
#include <glm/glm.hpp>
#include <vector>

class Gizmo : public SceneObject {
public:
    Gizmo();
    ~Gizmo();

    void Update(float deltaTime) override;
    void Render(Shader* shader) override;
    
    void SetPosition(const glm::vec3& position);
    void SetScale(float scale);
    void SetVisible(bool visible) { isVisible = visible; }
    bool IsVisible() const { return isVisible; }

private:
    void InitializeGizmo();
    
    GLuint VAO, VBO;
    std::vector<float> vertices;
    glm::vec3 position;
    float scale;
    bool isVisible;
    
    // Colors for each axis
    const glm::vec3 xAxisColor = glm::vec3(1.0f, 0.0f, 0.0f); // Red for X
    const glm::vec3 yAxisColor = glm::vec3(0.0f, 1.0f, 0.0f); // Green for Y
    const glm::vec3 zAxisColor = glm::vec3(0.0f, 0.0f, 1.0f); // Blue for Z
}; 