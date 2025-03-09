#pragma once

#include "Mesh.h"
#include "../Shader.h"
#include <vector>
#include <cmath>

class MeshFactory {
public:
    static Mesh* CreateCube(Shader* shader);
    static Mesh* CreateSphere(Shader* shader, float radius = 1.0f, int segments = 32);
    static Mesh* CreatePlane(Shader* shader, float width = 1.0f, float height = 1.0f);
    static Mesh* CreateCylinder(Shader* shader, float radius = 0.5f, float height = 2.0f, int segments = 32);
    
private:
    static std::vector<float> GetCubeVertices();
    static std::vector<unsigned int> GetCubeIndices();
    static void GenerateSphereData(std::vector<float>& vertices, std::vector<unsigned int>& indices, 
                                 float radius, int segments);
    static void GeneratePlaneData(std::vector<float>& vertices, std::vector<unsigned int>& indices, 
                                float width, float height);
    static void GenerateCylinderData(std::vector<float>& vertices, std::vector<unsigned int>& indices, 
                                   float radius, float height, int segments);
}; 