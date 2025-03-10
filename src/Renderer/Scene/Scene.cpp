#include "Scene/Scene.h"
#include "Scene/Mesh.h"
#include "Logger.h"
#include <algorithm>
#include <limits>

Scene::Scene(){
    // Set up isometric view position (similar to Blender's default view)
    cameraPosition = glm::vec3(7.3589f, 6.9258f, 4.9583f);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    zoomLevel = 11.0f;  // Adjusted for isometric distance
    yaw = -45.0f;   // Rotated 45 degrees for isometric
    pitch = -35.264f;  // Arctan(1/√2) for isometric angle
    activeMesh = nullptr;
    UpdateCameraVectors();
    SetProjection(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    // Add default light
    Light* defaultLight = new Light(
        glm::vec3(4.0f, 4.0f, 4.0f),    // Position
        glm::vec3(1.0f, 1.0f, 1.0f),    // Color
        1.0f                            // Intensity
    );
    AddLight(defaultLight);
}

Scene::~Scene() {
    for (SceneObject* obj : objects) {
        delete obj;
    }
    objects.clear();

    for (Light* light : lights) {
        delete light;
    }
    lights.clear();
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
    for (Light* light : lights) {
        light->Update(deltaTime);
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
    // Set view and projection matrices in the shader
    shader->SetViewMatrix(viewMatrix);
    shader->SetProjectionMatrix(projectionMatrix);
    
    // Set uniforms in the shader
    shader->Use();
    shader->SetUniformMat4("view", viewMatrix);
    shader->SetUniformMat4("projection", projectionMatrix);
    shader->SetUniform3f("viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    
    // Update and render lights
    for (Light* light : lights) {
        light->Update(0.0f);  // Update light visual representation
        light->Render(shader);
    }
    
    // Render all objects
    for (auto* object : objects) {
        object->Render(shader);
    }
}

SceneObject* Scene::PickMesh(const glm::vec3& rayOrigin, const glm::vec3& rayDir) {
    float closestT = std::numeric_limits<float>::max();
    SceneObject* closestMesh = nullptr;

    auto& logger = Logger::GetInstance();
    logger.Debug("Testing ray intersection with ", objects.size(), " objects");
    logger.Debug("Ray Origin: ", rayOrigin.x, ", ", rayOrigin.y, ", ", rayOrigin.z);
    logger.Debug("Ray Direction: ", rayDir.x, ", ", rayDir.y, ", ", rayDir.z);

    for (SceneObject* obj : objects) {
        Mesh* mesh = static_cast<Mesh*>(obj);
        
        // Get mesh transform data
        glm::vec3 pos = mesh->GetPosition();
        glm::vec3 scale = mesh->GetScale();
        
        // Create a larger bounding box for easier selection
        float boundSize = 2.0f; // Increased size for easier selection
        
        // Calculate bounds in world space
        glm::vec3 halfExtents = scale * boundSize;
        glm::vec3 min = pos - halfExtents;
        glm::vec3 max = pos + halfExtents;

        logger.Debug("Testing mesh at position: ", pos.x, ", ", pos.y, ", ", pos.z);
        logger.Debug("Box bounds: min(", min.x, ", ", min.y, ", ", min.z, 
                    ") max(", max.x, ", ", max.y, ", ", max.z, ")");

        // Ray-AABB intersection test using slab method
        float tmin = -std::numeric_limits<float>::infinity();
        float tmax = std::numeric_limits<float>::infinity();

        for (int i = 0; i < 3; ++i) {
            float invD = 1.0f / rayDir[i];
            float t0 = (min[i] - rayOrigin[i]) * invD;
            float t1 = (max[i] - rayOrigin[i]) * invD;

            if (invD < 0.0f) {
                std::swap(t0, t1);
            }

            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;

            if (tmax <= tmin) {
                logger.Debug("No intersection on axis ", i);
                break;
            }
        }

        // Check if we have a valid intersection
        if (tmin <= tmax && tmin > 0 && tmin < closestT) {
            closestT = tmin;
            closestMesh = obj;
            
            // Calculate and print intersection point
            glm::vec3 hitPoint = rayOrigin + rayDir * tmin;
            logger.Debug("Hit at distance: ", tmin);
            logger.Debug("Hit point: (", hitPoint.x, ", ", hitPoint.y, ", ", hitPoint.z, ")");
        }
    }

    if (closestMesh) {
        logger.Info("Selected mesh at distance: ", closestT);
    } else {
        logger.Info("No mesh selected");
    }

    return closestMesh;
}

void Scene::AddLight(Light* light) {
    lights.push_back(light);
}

void Scene::RemoveLight(Light* light) {
    lights.erase(std::remove(lights.begin(), lights.end(), light), lights.end());
    delete light;
}