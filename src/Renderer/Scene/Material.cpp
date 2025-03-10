#include "Scene/Material.h"
#include <algorithm>

Material::Material(const std::string& name) : name(name) {}

Material::~Material() {}

void Material::Apply(Shader* shader) {
    shader->SetUniform3f("material.albedo", albedo.x, albedo.y, albedo.z);
    shader->SetUniform1f("material.metallic", metallic);
    shader->SetUniform1f("material.roughness", roughness);
    shader->SetUniform1f("material.ao", ao);
    shader->SetUniform3f("material.emissive", emissive.x, emissive.y, emissive.z);
    shader->SetUniform1f("material.emissiveStrength", emissiveStrength);
}

Material* Material::CreateMetal() {
    Material* metal = new Material("Metal");
    metal->SetAlbedo(glm::vec3(0.95f, 0.95f, 0.95f));
    metal->SetMetallic(1.0f);
    metal->SetRoughness(0.2f);
    metal->SetAO(1.0f);
    return metal;
}

Material* Material::CreatePlastic() {
    Material* plastic = new Material("Plastic");
    plastic->SetAlbedo(glm::vec3(0.9f, 0.9f, 0.9f));
    plastic->SetMetallic(0.0f);
    plastic->SetRoughness(0.3f);
    plastic->SetAO(1.0f);
    return plastic;
}

Material* Material::CreateGlass() {
    Material* glass = new Material("Glass");
    glass->SetAlbedo(glm::vec3(0.95f, 0.95f, 0.95f));
    glass->SetMetallic(0.0f);
    glass->SetRoughness(0.05f);
    glass->SetAO(1.0f);
    return glass;
}

Material* Material::CreateWood() {
    Material* wood = new Material("Wood");
    wood->SetAlbedo(glm::vec3(0.6f, 0.35f, 0.2f));
    wood->SetMetallic(0.0f);
    wood->SetRoughness(0.9f);
    wood->SetAO(1.0f);
    return wood;
} 