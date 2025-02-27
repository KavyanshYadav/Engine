#include "Utiles.h"

void ShowErrorDialog(const char* message) {
	MessageBoxA(nullptr, message, "Error", MB_OK | MB_ICONERROR);
}

glm::vec3 ScreenToWorldRay(double mouseX, double mouseY, int screenWidth, int screenHeight, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    glm::vec4 rayNDC = glm::vec4(x, y, -1.0f, 1.0f);

    glm::vec4 rayView = glm::inverse(projectionMatrix) * rayNDC;
    rayView = glm::vec4(rayView.x, rayView.y, -1.0f, 0.0f); 

    glm::vec3 rayWorld = glm::vec3(glm::inverse(viewMatrix) * rayView);
    return glm::normalize(rayWorld); 
}

bool RayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, 
	const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t) {
	const float EPSILON = 0.0000001f;
	glm::vec3 edge1 = v1 - v0;
	glm::vec3 edge2 = v2 - v0;
	glm::vec3 h = glm::cross(rayDirection, edge2);
	float a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON) return false;

	float f = 1.0f / a;
	glm::vec3 s = rayOrigin - v0;
	float u = f * glm::dot(s, h);
	if (u < 0.0f || u > 1.0f) return false;

	glm::vec3 q = glm::cross(s, edge1);
	float v = f * glm::dot(rayDirection, q);
	if (v < 0.0f || u + v > 1.0f) return false;

	t = f * glm::dot(edge2, q);
	return t > EPSILON;
}

Mesh* PickMesh(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const std::vector<Mesh*>& meshes) {
    Mesh* selectedMesh = nullptr;
    // float closestT = std::numeric_limits<float>::max();

    // for (Mesh* mesh : meshes) {
    //     for (size_t i = 0; i < mesh->vertices.size(); i += 3) {
    //         float t;
    //         if (RayIntersectsTriangle(rayOrigin, rayDirection,
    //                                   mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2], t)) {
    //             if (t < closestT) {
    //                 closestT = t;
    //                 selectedMesh = mesh;
    //             }
    //         }
    //     }
    // }
    return selectedMesh;
}
