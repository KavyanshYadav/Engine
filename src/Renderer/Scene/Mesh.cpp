#include <glad/glad.h>
#include "Scene/Mesh.h"
#include <iostream>

Mesh::Mesh(Shader *shader):shader(shader) {
    position = glm::vec3(0.0f);
    rotationAngle = 0.0f;
    rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);  
    scale = glm::vec3(1.0f);
    // rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); 
    UpdateModelMatrix();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Mesh::LoadMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    indexCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Define vertex attributes (position only, 3 floats per vertex)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

}

void Mesh::LoadObj(const std::string& filename) {
    // tinyobj::attrib_t attrib;
    // std::vector<tinyobj::shape_t> shapes;
    // std::vector<tinyobj::material_t> materials;
    // std::string warn, err;

    // Load OBJ file
    // bool success = tinyobj::LoadObj()
    // if (!success) {
    //     std::cerr << "Error loading OBJ: " << err << std::endl;
   
    // }

    // if (!warn.empty()) {
    //     std::cout << "Warning: " << warn << std::endl;
    // }

    // std::vector<float> vertices;
    // std::vector<unsigned int> indices;
    // std::unordered_map<std::string, unsigned int> uniqueVertices;

    // for (const auto& shape : shapes) {
    //     for (const auto& index : shape.mesh.indices) {
    //         float vx = attrib.vertices[3 * index.vertex_index + 0];
    //         float vy = attrib.vertices[3 * index.vertex_index + 1];
    //         float vz = attrib.vertices[3 * index.vertex_index + 2];

    //         float nx = 0, ny = 0, nz = 0;
    //         if (!attrib.normals.empty() && index.normal_index >= 0) {
    //             nx = attrib.normals[3 * index.normal_index + 0];
    //             ny = attrib.normals[3 * index.normal_index + 1];
    //             nz = attrib.normals[3 * index.normal_index + 2];
    //         }

    //         float tx = 0, ty = 0;
    //         if (!attrib.texcoords.empty() && index.texcoord_index >= 0) {
    //             tx = attrib.texcoords[2 * index.texcoord_index + 0];
    //             ty = attrib.texcoords[2 * index.texcoord_index + 1];
    //         }

    //         std::string key = std::to_string(vx) + "_" + std::to_string(vy) + "_" + std::to_string(vz) +
    //                           "_" + std::to_string(nx) + "_" + std::to_string(ny) + "_" + std::to_string(nz) +
    //                           "_" + std::to_string(tx) + "_" + std::to_string(ty);

    //         if (uniqueVertices.count(key) == 0) {
    //             uniqueVertices[key] = static_cast<unsigned int>(vertices.size() / 8);
    //             vertices.insert(vertices.end(), {vx, vy, vz, nx, ny, nz, tx, ty});
    //         }

    //         indices.push_back(uniqueVertices[key]);
    //     }
    // }

}


void Mesh::Translate(const glm::vec3& translation) {
    position += translation;
    UpdateModelMatrix();
}
void Mesh::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;
    UpdateModelMatrix();
}

void Mesh::SetRotation(const glm::vec3& eulerAngles) {
    // rotation = glm::quat(glm::radians(eulerAngles)); 
    UpdateModelMatrix();
}

void Mesh::SetRotationQuat(const glm::quat& quatRotation) {
    // rotation = quatRotation;
    UpdateModelMatrix();
}

void Mesh::SetScale(const glm::vec3& newScale) {
    scale = newScale;
    UpdateModelMatrix();
}


void Mesh::Rotate(float angle, const glm::vec3& axis) {
    rotationAngle += angle;
    rotationAxis = axis;
    UpdateModelMatrix();
}

void Mesh::Scale(const glm::vec3& scaleFactor) {
    scale *= scaleFactor;
    UpdateModelMatrix();
}


void Mesh::UpdateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    modelMatrix = glm::scale(modelMatrix, scale);
}



void Mesh::Update(float deltaTime) {

}

void Mesh::Render(Shader *shader1) {
    // for (int i = 0; i < 4; ++i) {
    //     glm::vec4 row = glm::row(modelMatrix, i);
    //     std::cout << row.x << " " << row.y << " " << row.z << " " << row.w << std::endl;
    // }
    shader1->Use();
    shader1->SetUniformMat4("model",modelMatrix);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);  
    glBindVertexArray(0);
}