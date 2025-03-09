#pragma once
#include "../Shader.h"
#include "SceneObject.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <string>

// Forward declarations
class Material;

enum class MeshLayer {
    Default = 0,
    Background = 1,
    Environment = 2,
    Props = 3,
    Characters = 4,
    UI = 5,
    Effects = 6
};

struct MeshStats {
    uint32_t vertexCount;
    uint32_t triangleCount;
    uint32_t materialCount;
    float boundingSphereRadius;
};

class Mesh : public SceneObject {
public:
    Mesh(Shader *shader);
    ~Mesh();
    
    // void LoadMesh(const std::vector<float>& vertices, 
    //     const std::vector<unsigned int>& indices, 
    //     const std::vector<glm::vec3>& normals, 
    //     const std::vector<glm::vec2>& texCoords, 
    //     const std::vector<glm::vec3>& tangents, 
    //     const std::vector<glm::vec3>& bitangents);
    void LoadObj(const std::string& filename);

    void LoadMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    void Update(float deltaTime) override;
    void Render(Shader *shader1) override;

    void Translate(const glm::vec3& translation);
    void Rotate(float angle, const glm::vec3& axis);
    void Scale(const glm::vec3& scaleFactor);

    void SetPosition(const glm::vec3& newPosition) ;
    void SetRotation(const glm::vec3& eulerAngles);
    void SetRotationQuat(const glm::quat& quatRotation);
    void SetScale(const glm::vec3& newScale) ;

    glm::vec3 GetPosition() const  { return position; }
    glm::vec3 GetRotation() const { return rotation; }
    glm::vec3 GetScale() const  { return scale; }
    glm::mat4 GetModelMatrix() const { return modelMatrix; }
    const MeshStats& GetStats() const { return stats; }

    // New features
    void SetName(const std::string& name) { this->name = name; }
    const std::string& GetName() const { return name; }
    
    void SetLayer(MeshLayer layer) { this->layer = layer; }
    MeshLayer GetLayer() const { return layer; }
    
    void SetVisible(bool visible) { isVisible = visible; }
    bool IsVisible() const { return isVisible; }
    
    void SetStatic(bool isStatic) { this->isStatic = isStatic; }
    bool IsStatic() const { return isStatic; }
    
    void SetCastShadows(bool cast) { castShadows = cast; }
    bool CastShadows() const { return castShadows; }
    
    void SetReceiveShadows(bool receive) { receiveShadows = receive; }
    bool ReceiveShadows() const { return receiveShadows; }

    // Material system
    void AddMaterial(Material* material);
    void RemoveMaterial(Material* material);
    const std::vector<Material*>& GetMaterials() const { return materials; }
    
    // Parent-child hierarchy
    void SetParent(Mesh* parent);
    Mesh* GetParent() const { return parent; }
    void AddChild(Mesh* child);
    void RemoveChild(Mesh* child);
    const std::vector<Mesh*>& GetChildren() const { return children; }
    
    // Physics properties
    void SetMass(float mass) { this->mass = mass; }
    float GetMass() const { return mass; }
    void SetDynamic(bool dynamic) { isDynamic = dynamic; }
    bool IsDynamic() const { return isDynamic; }
    
    // Animation
    void SetAnimated(bool animated) { isAnimated = animated; }
    bool IsAnimated() const { return isAnimated; }
    void UpdateAnimation(float deltaTime);

private:
    GLuint VAO, VBO, EBO;  
    GLuint indexCount;
    Shader *shader;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 modelMatrix;
    void UpdateModelMatrix();
    void UpdateStats();

    // New features
    std::string name;
    MeshLayer layer;
    bool isVisible;
    bool isStatic;
    bool castShadows;
    bool receiveShadows;
    bool isDynamic;
    bool isAnimated;
    
    // Physics properties
    float mass;
    
    // Parent-child hierarchy
    Mesh* parent;
    std::vector<Mesh*> children;
    
    // Rendering properties
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<Material*> materials;
    MeshStats stats;
    
    // Animation data
    float animationTime;
};
