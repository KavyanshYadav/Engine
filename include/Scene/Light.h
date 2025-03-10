#pragma once
#include "SceneObject.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class Light : public SceneObject {
public:
    Light(const glm::vec3& position = glm::vec3(4.0f, 4.0f, 4.0f),
          const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f),
          float intensity = 1.0f);
    ~Light();
    
    void Update(float deltaTime) override;
    void Render(Shader* shader) override;

    // Getters
    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetColor() const { return color; }
    float GetIntensity() const { return intensity; }

    // Setters
    void SetPosition(const glm::vec3& pos);
    void SetColor(const glm::vec3& col);
    void SetIntensity(float i) { intensity = i; }

    // Visual representation
    Mesh* GetVisualMesh() const { return visualMesh; }

private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    Mesh* visualMesh;  // Visual representation of the light
    void UpdateVisualMesh();
}; 