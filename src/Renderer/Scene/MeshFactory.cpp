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
        // Front face         // Colors
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  // Bottom-left - Red
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-right - Green
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  // Top-right - Blue
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  // Top-left - Yellow
        
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  // Bottom-left - Cyan
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,  // Bottom-right - Magenta
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  // Top-right - White
        -0.5f,  0.5f, -0.5f, 0.5f, 0.5f, 0.5f   // Top-left - Gray
    };
}

std::vector<unsigned int> MeshFactory::GetCubeIndices() {
    return {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Back face
        5, 4, 7,
        7, 6, 5,
        // Left face
        4, 0, 3,
        3, 7, 4,
        // Top face
        3, 2, 6,
        6, 7, 3,
        // Bottom face
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
    
    // Center vertices (for caps)
    // Top center
    vertices.push_back(0.0f);          // Position
    vertices.push_back(halfHeight);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);          // Color (white)
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);

    // Bottom center
    vertices.push_back(0.0f);          // Position
    vertices.push_back(-halfHeight);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);          // Color (white)
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);

    // Generate vertices for the sides
    for (int i = 0; i <= segments; i++) {
        float angle = i * 2.0f * M_PI / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        float r = cos(angle) * 0.5f + 0.5f;
        float g = sin(angle) * 0.5f + 0.5f;
        float b = 0.5f;

        // Top vertex
        vertices.push_back(x);           // Position
        vertices.push_back(halfHeight);
        vertices.push_back(z);
        vertices.push_back(r);           // Color
        vertices.push_back(g);
        vertices.push_back(b);

        // Bottom vertex
        vertices.push_back(x);           // Position
        vertices.push_back(-halfHeight);
        vertices.push_back(z);
        vertices.push_back(r);           // Color
        vertices.push_back(g);
        vertices.push_back(b);
    }

    // Generate indices for the sides
    for (int i = 0; i < segments; i++) {
        int baseIndex = 2 + (i * 2); // +2 for the center vertices
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);

        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 3);
        indices.push_back(baseIndex + 2);
    }

    // Generate indices for caps
    for (int i = 0; i < segments; i++) {
        int topIndex = 2 + (i * 2);
        int nextTopIndex = 2 + ((i + 1) % segments) * 2;
        
        // Top cap
        indices.push_back(0);  // Center top
        indices.push_back(topIndex);
        indices.push_back(nextTopIndex);

        // Bottom cap
        indices.push_back(1);  // Center bottom
        indices.push_back(topIndex + 1);
        indices.push_back(nextTopIndex + 1);
    }
} 