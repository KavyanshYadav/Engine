#include "Scene/Scene.h"
#include "Scene/Mesh.h"
#include <algorithm>
#include <limits>
#include <iostream>

Scene::Scene(){
    cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    zoomLevel = 3.0f;  // Initial zoom level
    yaw = -90.0f;   // Start looking along negative Z
    pitch = 0.0f;
    activeMesh = nullptr;
    UpdateCameraVectors();
    SetProjection(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
}

Scene::~Scene() {
    for (SceneObject* obj : objects) {
        delete obj;
    }
    objects.clear();
}

void Scene::AddObject(SceneObject* object) {
    objects.push_back(object);
}

void Scene::RemoveObject(SceneObject* object) {
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}

void Scene::SetCameraView(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
    cameraPosition = position;
    cameraTarget = target;
    cameraUp = up;
    viewMatrix = glm::lookAt(position, target, up);
}

void Scene::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane) {
    projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

void Scene::Update(float deltaTime) {
    for (SceneObject* obj : objects) {
        obj->Update(deltaTime);
    }
}

void Scene::AdjustCameraZoom(float zoomDelta) {
    // Adjust zoom level with some constraints
    zoomLevel = glm::clamp(zoomLevel - zoomDelta, 1.0f, 10.0f);
    
    // Calculate new camera position based on zoom level
    glm::vec3 direction = glm::normalize(cameraPosition - cameraTarget);
    cameraPosition = cameraTarget + direction * zoomLevel;
    
    // Update view matrix
    viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
}

void Scene::RotateCamera(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw;
    pitch += deltaPitch;
    
    // Constrain pitch to avoid camera flipping
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    
    UpdateCameraVectors();
}

void Scene::MoveCamera(const glm::vec3& direction) {
    float speed = 0.1f; // Adjust movement speed as needed
    glm::vec3 front = glm::normalize(cameraTarget - cameraPosition);
    glm::vec3 right = glm::normalize(glm::cross(front, cameraUp));
    
    cameraPosition += direction.z * front * speed;
    cameraPosition += direction.x * right * speed;
    cameraPosition += direction.y * cameraUp * speed;
    
    cameraTarget = cameraPosition + front;
    UpdateCameraVectors();
}

void Scene::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    cameraTarget = cameraPosition + glm::normalize(front);
    viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
}

std::vector<SceneObject*> Scene::GetSceneNodes (){
    return this->objects;
}

void Scene::Render(Shader* shader) {
    shader->Use();

    shader->SetUniformMat4("view", viewMatrix);
    shader->SetUniformMat4("projection", projectionMatrix);
    for (SceneObject* obj : objects) {
        obj->Render(shader);
        
    }
}

SceneObject* Scene::PickMesh(const glm::vec3& rayOrigin, const glm::vec3& rayDir) {
    float closestT = std::numeric_limits<float>::max();
    SceneObject* closestMesh = nullptr;

    for (SceneObject* obj : objects) {
        Mesh* mesh = static_cast<Mesh*>(obj);
        // Get mesh bounds with some margin for easier selection
        glm::vec3 pos = mesh->GetPosition();
        glm::vec3 scale = mesh->GetScale();
        float boundSize = 1.0f; // Size of selection bounds
        
        glm::vec3 min = pos - (scale * boundSize);
        glm::vec3 max = pos + (scale * boundSize);

        // Ray-AABB intersection test
        float tMin = -std::numeric_limits<float>::max();
        float tMax = std::numeric_limits<float>::max();
        bool hit = true;

        for (int i = 0; i < 3; i++) {
            if (abs(rayDir[i]) < std::numeric_limits<float>::epsilon()) {
                // Ray is parallel to slab
                if (rayOrigin[i] < min[i] || rayOrigin[i] > max[i]) {
                    hit = false;
                    break;
                }
            } else {
                float invD = 1.0f / rayDir[i];
                float t1 = (min[i] - rayOrigin[i]) * invD;
                float t2 = (max[i] - rayOrigin[i]) * invD;

                if (t1 > t2) std::swap(t1, t2);

                tMin = std::max(tMin, t1);
                tMax = std::min(tMax, t2);

                if (tMin > tMax) {
                    hit = false;
                    break;
                }
            }
        }

        // If we hit and it's closer than previous hits
        if (hit && tMin < closestT && tMin > 0) {
            closestT = tMin;
            closestMesh = obj;
            std::cout << "Hit mesh at distance: " << tMin << std::endl;
        }
    }

    return closestMesh;
}