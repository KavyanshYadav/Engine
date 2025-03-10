#version 450 core

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    vec3 emissive;
    float emissiveStrength;
};

in vec3 vertexColor;
out vec4 FragColor;

uniform Material material;

void main() {
    // For now, we'll use a simple combination of material properties and vertex colors
    vec3 baseColor = vertexColor * material.albedo;
    vec3 emissiveContribution = material.emissive * material.emissiveStrength;
    
    // Simple metallic-roughness influence
    vec3 finalColor = mix(baseColor, baseColor * (1.0 - material.roughness), material.metallic);
    finalColor = finalColor * material.ao + emissiveContribution;
    
    FragColor = vec4(finalColor, 1.0);
}  