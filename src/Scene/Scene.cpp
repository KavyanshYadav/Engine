#include "Scene/Scene.h"
#include "Scene/Mesh.h"
#include "Utiles.h"
#include <iostream>

// Initialize static members
DebugState Scene::debugState;
Shader* Scene::boundingBoxShader = nullptr;

Scene::Scene() : activeMesh(nullptr), cameraPosition(0.0f, 0.0f, 3.0f), 
                cameraTarget(0.0f, 0.0f, 0.0f), cameraUp(0.0f, 1.0f, 0.0f),
                zoomLevel(1.0f), yaw(-90.0f), pitch(0.0f) {
    // Initialize view and projection matrices
    viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Initialize gizmo shader
    gizmoShader = new Shader("SHADERS/gizmo.vert", "SHADERS/gizmo.frag");
    gizmoShader->CreateShaderProgram();

    // Initialize bounding box shader
    if (!boundingBoxShader) {
        boundingBoxShader = new Shader("SHADERS/boundingbox.vert", "SHADERS/boundingbox.frag");
        boundingBoxShader->CreateShaderProgram();
    }

    // Initialize gizmo
    gizmo = new Gizmo();
    gizmo->SetScale(0.5f);
    gizmo->SetVisible(false);
}

Scene::~Scene() {
    for (auto object : objects) {
        delete object;
    }
    for (auto light : lights) {
        delete light;
    }
    delete gizmoShader;
    delete gizmo;

    // Delete bounding box shader
    if (boundingBoxShader) {
        delete boundingBoxShader;
        boundingBoxShader = nullptr;
    }
}

void Scene::AddObject(SceneObject* object) {
    objects.push_back(object);
}

void Scene::RemoveObject(SceneObject* object) {
    auto it = std::find(objects.begin(), objects.end(), object);
    if (it != objects.end()) {
        objects.erase(it);
    }
}

void Scene::AddLight(Light* light) {
    lights.push_back(light);
}

void Scene::RemoveLight(Light* light) {
    auto it = std::find(lights.begin(), lights.end(), light);
    if (it != lights.end()) {
        lights.erase(it);
    }
}

void Scene::Update(float deltaTime) {
    for (auto object : objects) {
        object->Update(deltaTime);
    }
    for (auto light : lights) {
        light->Update(deltaTime);
    }

    // Update gizmo position if there's an active mesh
    if (activeMesh && gizmo->IsVisible()) {
        if (auto mesh = dynamic_cast<Mesh*>(activeMesh)) {
            gizmo->SetPosition(mesh->GetPosition());
        }
    }
}

void Scene::Render(Shader* shader) {
    // Update view and projection matrices in shader
    shader->SetViewMatrix(viewMatrix);
    shader->SetProjectionMatrix(projectionMatrix);

    // Update bounding box shader matrices
    if (boundingBoxShader) {
        boundingBoxShader->Use();
        boundingBoxShader->SetViewMatrix(viewMatrix);
        boundingBoxShader->SetProjectionMatrix(projectionMatrix);
    }

    // Render scene objects
    for (auto object : objects) {
        object->Render(shader);
    }

    // Render lights
    for (auto light : lights) {
        light->Render(shader);
    }

    // Render gizmo if visible and there's an active mesh
    if (gizmo->IsVisible() && activeMesh) {
        gizmoShader->SetViewMatrix(viewMatrix);
        gizmoShader->SetProjectionMatrix(projectionMatrix);
        gizmo->Render(gizmoShader);
    }
}

void Scene::SetactiveMesh(SceneObject* mesh) {
    // If we had a previously active mesh, deselect it
    if (activeMesh) {
        if (auto prevMesh = dynamic_cast<Mesh*>(activeMesh)) {
            prevMesh->SetSelected(false);
        }
    }

    activeMesh = mesh;

    // Select the new mesh and show gizmo
    if (activeMesh) {
        if (auto newMesh = dynamic_cast<Mesh*>(activeMesh)) {
            newMesh->SetSelected(true);
            gizmo->SetVisible(true);
            gizmo->SetPosition(newMesh->GetPosition());
        }
    } else {
        gizmo->SetVisible(false);
    }
}

// ... rest of the Scene class implementation ...

void Scene::SetCameraView(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
    cameraPosition = position;
    cameraTarget = target;
    cameraUp = up;
    UpdateCameraVectors();
}

void Scene::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane) {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Scene::AdjustCameraZoom(float zoomDelta) {
    zoomLevel = glm::clamp(zoomLevel - zoomDelta * 0.1f, 0.1f, 10.0f);
    UpdateCameraVectors();
}

void Scene::RotateCamera(float newYaw, float newPitch) {
    yaw += newYaw;
    pitch = glm::clamp(pitch + newPitch, -89.0f, 89.0f);
    UpdateCameraVectors();
}

void Scene::MoveCamera(const glm::vec3& direction) {
    cameraPosition += direction;
    cameraTarget += direction;
    UpdateCameraVectors();
}

void Scene::UpdateCameraVectors() {
    // Calculate the new camera position based on angle and zoom
    float radius = glm::distance(cameraPosition, cameraTarget) * zoomLevel;
    float x = radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    float y = radius * sin(glm::radians(pitch));
    float z = radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    cameraPosition = cameraTarget + glm::vec3(x, y, z);
    viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
}

std::vector<SceneObject*> Scene::GetSceneNodes() {
    return objects;
}

SceneObject* Scene::PickMesh(const glm::vec3& rayOrigin, const glm::vec3& rayDir) {
    std::vector<Mesh*> meshes;
    for (auto object : objects) {
        if (auto mesh = dynamic_cast<Mesh*>(object)) {
            meshes.push_back(mesh);
        }
    }
    return ::PickMesh(rayOrigin, rayDir, meshes);
} 