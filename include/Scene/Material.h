#pragma once
#include <glm/glm.hpp>
#include "../Shader.h"
#include <string>

class Material {
public:
    Material(const std::string& name = "Default Material");
    ~Material();

    // PBR material properties
    void SetAlbedo(const glm::vec3& color) { albedo = color; }
    void SetMetallic(float value) { metallic = glm::clamp(value, 0.0f, 1.0f); }
    void SetRoughness(float value) { roughness = glm::clamp(value, 0.0f, 1.0f); }
    void SetAO(float value) { ao = glm::clamp(value, 0.0f, 1.0f); }
    void SetEmissive(const glm::vec3& value) { emissive = value; }
    void SetEmissiveStrength(float value) { emissiveStrength = value; }

    // Getters
    const glm::vec3& GetAlbedo() const { return albedo; }
    float GetMetallic() const { return metallic; }
    float GetRoughness() const { return roughness; }
    float GetAO() const { return ao; }
    const glm::vec3& GetEmissive() const { return emissive; }
    float GetEmissiveStrength() const { return emissiveStrength; }
    const std::string& GetName() const { return name; }

    // Setters
    void SetName(const std::string& newName) { name = newName; }

    // Apply material properties to shader
    void Apply(Shader* shader);

    // Preset materials
    static Material* CreateMetal();
    static Material* CreatePlastic();
    static Material* CreateGlass();
    static Material* CreateWood();

private:
    std::string name;
    
    // PBR properties
    glm::vec3 albedo{0.8f};           // Base color
    float metallic{0.0f};             // Metallic value (0 = dielectric, 1 = metal)
    float roughness{0.5f};            // Surface roughness
    float ao{1.0f};                   // Ambient occlusion
    glm::vec3 emissive{0.0f};         // Emissive color
    float emissiveStrength{0.0f};     // Emissive strength multiplier
}; 