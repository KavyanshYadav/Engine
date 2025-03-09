#include "Scene/MeshFactory.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Mesh* MeshFactory::CreateCube(Shader* shader) {
    Mesh* cube = new Mesh(shader);
    cube->LoadMesh(GetCubeVertices(), GetCubeIndices());
    return cube;
}

Mesh* MeshFactory::CreateSphere(Shader* shader, float radius, int segments) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    GenerateSphereData(vertices, indices, radius, segments);
    
    Mesh* sphere = new Mesh(shader);
    sphere->LoadMesh(vertices, indices);
    return sphere;
}

Mesh* MeshFactory::CreatePlane(Shader* shader, float width, float height) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    GeneratePlaneData(vertices, indices, width, height);
    
    Mesh* plane = new Mesh(shader);
    plane->LoadMesh(vertices, indices);
    return plane;
}

Mesh* MeshFactory::CreateCylinder(Shader* shader, float radius, float height, int segments) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    GenerateCylinderData(vertices, indices, radius, height, segments);
    
    Mesh* cylinder = new Mesh(shader);
    cylinder->LoadMesh(vertices, indices);
    return cylinder;
}

std::vector<float> MeshFactory::GetCubeVertices() {
    return {
        // Front face
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // Back face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };
}

std::vector<unsigned int> MeshFactory::GetCubeIndices() {
    return {
        // Front
        0, 1, 2,
        2, 3, 0,
        // Right
        1, 5, 6,
        6, 2, 1,
        // Back
        5, 4, 7,
        7, 6, 5,
        // Left
        4, 0, 3,
        3, 7, 4,
        // Top
        3, 2, 6,
        6, 7, 3,
        // Bottom
        4, 5, 1,
        1, 0, 4
    };
}

void MeshFactory::GenerateSphereData(std::vector<float>& vertices, std::vector<unsigned int>& indices, 
                                   float radius, int segments) {
    // Generate vertices
    for (int lat = 0; lat <= segments; lat++) {
        float theta = lat * M_PI / segments;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= segments; lon++) {
            float phi = lon * 2 * M_PI / segments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);
        }
    }

    // Generate indices
    for (int lat = 0; lat < segments; lat++) {
        for (int lon = 0; lon < segments; lon++) {
            int first = (lat * (segments + 1)) + lon;
            int second = first + segments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void MeshFactory::GeneratePlaneData(std::vector<float>& vertices, std::vector<unsigned int>& indices, 
                                  float width, float height) {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // Vertices
    vertices = {
        -halfWidth, 0.0f, -halfHeight,  // 0
         halfWidth, 0.0f, -halfHeight,  // 1
         halfWidth, 0.0f,  halfHeight,  // 2
        -halfWidth, 0.0f,  halfHeight   // 3
    };

    // Indices
    indices = {
        0, 1, 2,
        2, 3, 0
    };
}

void MeshFactory::GenerateCylinderData(std::vector<float>& vertices, std::vector<unsigned int>& indices, 
                                     float radius, float height, int segments) {
    float halfHeight = height / 2.0f;

    // Generate vertices for the sides
    for (int i = 0; i <= segments; i++) {
        float angle = i * 2.0f * M_PI / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Top vertex
        vertices.push_back(x);
        vertices.push_back(halfHeight);
        vertices.push_back(z);

        // Bottom vertex
        vertices.push_back(x);
        vertices.push_back(-halfHeight);
        vertices.push_back(z);
    }

    // Generate indices for the sides
    for (int i = 0; i < segments; i++) {
        int baseIndex = i * 2;
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);

        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 3);
        indices.push_back(baseIndex + 2);
    }

    // Add center vertices for top and bottom caps
    int centerTopIndex = vertices.size() / 3;
    vertices.push_back(0.0f);
    vertices.push_back(halfHeight);
    vertices.push_back(0.0f);

    int centerBottomIndex = centerTopIndex + 1;
    vertices.push_back(0.0f);
    vertices.push_back(-halfHeight);
    vertices.push_back(0.0f);

    // Generate indices for caps
    for (int i = 0; i < segments; i++) {
        // Top cap
        indices.push_back(centerTopIndex);
        indices.push_back(i * 2);
        indices.push_back(((i + 1) % segments) * 2);

        // Bottom cap
        indices.push_back(centerBottomIndex);
        indices.push_back(i * 2 + 1);
        indices.push_back(((i + 1) % segments) * 2 + 1);
    }
} 