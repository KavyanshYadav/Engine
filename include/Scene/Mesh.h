#pragma once
#include "SceneObject.h"
#include <vector>

class Mesh : public SceneObject {
public:
    Mesh();
    ~Mesh();

    void LoadMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    void Update(float deltaTime) override;
    void Render() override;

private:
    GLuint VAO, VBO, EBO;  
    GLuint indexCount;
};
