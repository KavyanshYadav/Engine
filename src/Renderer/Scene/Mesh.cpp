#include <glad/glad.h>
#include "Scene/Mesh.h"
#include "Scene/Material.h"
#include "Scene/Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/component_wise.hpp>
#include <iostream>

Mesh::Mesh(Shader* shader) : shader(shader) {
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    modelMatrix = glm::mat4(1.0f);
    
    // Initialize default values
    name = "New Mesh";
    layer = MeshLayer::Default;
    isVisible = true;
    isStatic = false;
    castShadows = true;
    receiveShadows = true;
    isDynamic = false;
    isAnimated = false;
    mass = 1.0f;
    parent = nullptr;
    animationTime = 0.0f;

    // Initialize outline properties
    isSelected = false;
    outlineColor = glm::vec3(1.0f, 0.5f, 0.0f); // Brighter orange outline color
    outlineScale = 0.05f; // Increased scale for more visible outline

    // Create outline shader
    outlineShader = new Shader("SHADERS/outline.vert", "SHADERS/outline.frag");
    outlineShader->CreateShaderProgram();

    // Initialize OpenGL buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Initialize bounding box
    boundingBoxInitialized = false;
    minBounds = glm::vec3(0.0f);
    maxBounds = glm::vec3(0.0f);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete outlineShader;
}

void Mesh::LoadMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    this->vertices = vertices;
    this->indices = indices;
    indexCount = static_cast<GLuint>(indices.size());

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    UpdateStats();
    
    // Calculate bounding box
    if (!vertices.empty()) {
        minBounds = glm::vec3(vertices[0], vertices[1], vertices[2]);
        maxBounds = minBounds;

        // Iterate through vertices (stride of 6 because each vertex has position and normal)
        for (size_t i = 0; i < vertices.size(); i += 6) {
            glm::vec3 pos(vertices[i], vertices[i + 1], vertices[i + 2]);
            minBounds = glm::min(minBounds, pos);
            maxBounds = glm::max(maxBounds, pos);
        }
        InitializeBoundingBox();
    }
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

void Mesh::Update(float deltaTime) {
    if (isAnimated) {
        UpdateAnimation(deltaTime);
    }
}

void Mesh::Render(Shader* shader) {
    if (!isVisible) return;

    // Normal mesh rendering
    shader->Use();
    shader->SetUniformMat4("model", GetModelMatrix());
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Render bounding box if debug mode is enabled
    if (Scene::GetDebugState().showBoundingBoxes && Scene::GetBoundingBoxShader()) {
        RenderBoundingBox(Scene::GetBoundingBoxShader());
    }
}

void Mesh::Translate(const glm::vec3& translation) {
    position += translation;
    UpdateModelMatrix();
}

void Mesh::Rotate(float angle, const glm::vec3& axis) {
    rotation += axis * angle;
    UpdateModelMatrix();
}

void Mesh::Scale(const glm::vec3& scaleFactor) {
    scale *= scaleFactor;
    UpdateModelMatrix();
}

void Mesh::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;
    UpdateModelMatrix();
}

void Mesh::SetRotation(const glm::vec3& eulerAngles) {
    rotation = eulerAngles;
    UpdateModelMatrix();
}

void Mesh::SetRotationQuat(const glm::quat& quatRotation) {
    rotation = glm::eulerAngles(quatRotation);
    UpdateModelMatrix();
}

void Mesh::SetScale(const glm::vec3& newScale) {
    scale = newScale;
    UpdateModelMatrix();
}

void Mesh::UpdateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
}

void Mesh::UpdateStats() {
    stats.vertexCount = vertices.size() / 3;  // Assuming 3 components per vertex
    stats.triangleCount = indices.size() / 3;
    stats.materialCount = materials.size();
    
    // Calculate bounding sphere radius
    float maxDistSq = 0.0f;
    for (size_t i = 0; i < vertices.size(); i += 3) {
        glm::vec3 vertex(vertices[i], vertices[i + 1], vertices[i + 2]);
        float distSq = glm::dot(vertex, vertex);  // Calculate squared length using dot product
        maxDistSq = std::max(maxDistSq, distSq);
    }
    stats.boundingSphereRadius = std::sqrt(maxDistSq);
}

void Mesh::AddMaterial(Material* material) {
    materials.push_back(material);
    UpdateStats();
}

void Mesh::RemoveMaterial(Material* material) {
    auto it = std::find(materials.begin(), materials.end(), material);
    if (it != materials.end()) {
        materials.erase(it);
        UpdateStats();
    }
}

void Mesh::SetParent(Mesh* newParent) {
    if (parent) {
        parent->RemoveChild(this);
    }
    parent = newParent;
    if (parent) {
        parent->AddChild(this);
    }
}

void Mesh::AddChild(Mesh* child) {
    if (child && std::find(children.begin(), children.end(), child) == children.end()) {
        children.push_back(child);
    }
}

void Mesh::RemoveChild(Mesh* child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
    }
}

void Mesh::UpdateAnimation(float deltaTime) {
    if (!isAnimated) return;
    animationTime += deltaTime;
    // Animation implementation will go here
}

void Mesh::SetSelected(bool selected) {
    isSelected = selected;
}

void Mesh::InitializeBoundingBox() {
    // Calculate bounds
    if (vertices.empty()) return;

    // Initialize bounds with first vertex (stride of 6: 3 for position, 3 for normal)
    minBounds = glm::vec3(vertices[0], vertices[1], vertices[2]);
    maxBounds = minBounds;

    // Iterate through vertices (stride of 6 because each vertex has position and normal)
    for (size_t i = 0; i < vertices.size(); i += 6) {
        glm::vec3 pos(vertices[i], vertices[i + 1], vertices[i + 2]);
        minBounds.x = std::min(minBounds.x, pos.x);
        minBounds.y = std::min(minBounds.y, pos.y);
        minBounds.z = std::min(minBounds.z, pos.z);
        maxBounds.x = std::max(maxBounds.x, pos.x);
        maxBounds.y = std::max(maxBounds.y, pos.y);
        maxBounds.z = std::max(maxBounds.z, pos.z);
    }

    // Create vertices for bounding box (12 lines = 24 vertices)
    boundingBoxVertices = {
        // Front face
        minBounds.x, minBounds.y, minBounds.z,
        maxBounds.x, minBounds.y, minBounds.z,
        
        maxBounds.x, minBounds.y, minBounds.z,
        maxBounds.x, maxBounds.y, minBounds.z,
        
        maxBounds.x, maxBounds.y, minBounds.z,
        minBounds.x, maxBounds.y, minBounds.z,
        
        minBounds.x, maxBounds.y, minBounds.z,
        minBounds.x, minBounds.y, minBounds.z,

        // Back face
        minBounds.x, minBounds.y, maxBounds.z,
        maxBounds.x, minBounds.y, maxBounds.z,
        
        maxBounds.x, minBounds.y, maxBounds.z,
        maxBounds.x, maxBounds.y, maxBounds.z,
        
        maxBounds.x, maxBounds.y, maxBounds.z,
        minBounds.x, maxBounds.y, maxBounds.z,
        
        minBounds.x, maxBounds.y, maxBounds.z,
        minBounds.x, minBounds.y, maxBounds.z,

        // Connecting lines
        minBounds.x, minBounds.y, minBounds.z,
        minBounds.x, minBounds.y, maxBounds.z,
        
        maxBounds.x, minBounds.y, minBounds.z,
        maxBounds.x, minBounds.y, maxBounds.z,
        
        maxBounds.x, maxBounds.y, minBounds.z,
        maxBounds.x, maxBounds.y, maxBounds.z,
        
        minBounds.x, maxBounds.y, minBounds.z,
        minBounds.x, maxBounds.y, maxBounds.z
    };

    // Create and setup VAO/VBO for bounding box
    glGenVertexArrays(1, &boundingBoxVAO);
    glGenBuffers(1, &boundingBoxVBO);

    glBindVertexArray(boundingBoxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boundingBoxVBO);
    glBufferData(GL_ARRAY_BUFFER, boundingBoxVertices.size() * sizeof(float), boundingBoxVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    boundingBoxInitialized = true;
}

void Mesh::UpdateBoundingBox() {
    // Update bounds based on current transformation
    glm::mat4 model = GetModelMatrix();
    glm::vec3 transformedMin = glm::vec3(model * glm::vec4(minBounds, 1.0f));
    glm::vec3 transformedMax = glm::vec3(model * glm::vec4(maxBounds, 1.0f));

    // Update bounding box vertices with transformed coordinates
    // ... (similar to InitializeBoundingBox but with transformed coordinates)
}

void Mesh::RenderBoundingBox(Shader* shader) const {
    if (!boundingBoxInitialized) return;

    shader->Use();
    shader->SetUniformMat4("model", GetModelMatrix());

    // Enable line rendering
    glLineWidth(2.0f);
    
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(boundingBoxVAO);
    glDrawArrays(GL_LINES, 0, 24); // 12 lines = 24 vertices
    glBindVertexArray(0);

    // Reset line width and disable blending
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
}