#pragma once 


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene/Mesh.h"
#ifdef _WIN32
#include <windows.h>


void ShowErrorDialog(const char* message);
#endif


// RayCasting Algorithm --- needs optimizations - OP
glm::vec3 ScreenToWorldRay(double mouseX, double mouseY, int screenWidth, int screenHeight, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
bool RayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, 
const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t) ;
Mesh* PickMesh(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const std::vector<Mesh*>& meshes);