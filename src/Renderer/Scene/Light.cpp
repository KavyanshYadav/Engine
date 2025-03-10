#include "Scene/Light.h"
#include "Scene/MeshFactory.h"
#include <glad/glad.h>

Light::Light(const glm::vec3& position, const glm::vec3& color, float intensity)
    : position(position), color(color), intensity(intensity) {
    // Visual mesh will be initialized in Render when we have a valid shader
    visualMesh = nullptr;
}

Light::~Light() {
    if (visualMesh) {
        delete visualMesh;
        visualMesh = nullptr;
    }
}

void Light::Update(float deltaTime) {
    UpdateVisualMesh();
}

void Light::Render(Shader* shader) {
    // Set light uniforms in shader
    shader->SetUniform3f("lightPos", position.x, position.y, position.z);
    shader->SetUniform3f("lightColor", color.x * intensity, color.y * intensity, color.z * intensity);
    
    // Create visual mesh if it doesn't exist
    if (!visualMesh && shader) {
        visualMesh = MeshFactory::CreateSphere(shader, 0.2f, 16);
        visualMesh->SetPosition(position);
        
        // Create an emissive material for the light
        Material* lightMaterial = new Material("Light Material");
        lightMaterial->SetEmissive(color);
        lightMaterial->SetEmissiveStrength(intensity);
        visualMesh->AddMaterial(lightMaterial);
    }
    
    // Render the visual representation
    if (visualMesh) {
        visualMesh->Render(shader);
    }
}

void Light::SetPosition(const glm::vec3& pos) {
    position = pos;
    UpdateVisualMesh();
}

void Light::SetColor(const glm::vec3& col) {
    color = col;
    if (visualMesh && !visualMesh->GetMaterials().empty()) {
        visualMesh->GetMaterials()[0]->SetEmissive(col);
    }
}

void Light::UpdateVisualMesh() {
    if (visualMesh) {
        visualMesh->SetPosition(position);
        if (!visualMesh->GetMaterials().empty()) {
            visualMesh->GetMaterials()[0]->SetEmissiveStrength(intensity);
        }
    }
} 