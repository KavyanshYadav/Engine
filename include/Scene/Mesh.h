#pragma once
#include "../Shader.h"
#include "SceneObject.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>


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

    void SetPosition(const glm::vec3& newPosition);
    void SetRotation(const glm::vec3& eulerAngles);
    void SetRotationQuat(const glm::quat& quatRotation);
    void SetScale(const glm::vec3& newScale);

    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetScale() const { return scale; }

private:
    GLuint VAO, VBO, EBO;  
    GLuint indexCount;
    Shader *shader;
    glm::vec3 position;
    glm::vec3 rotationAxis;
    float rotationAngle;
    glm::vec3 scale;

    glm::mat4 modelMatrix;
    void UpdateModelMatrix();

};
