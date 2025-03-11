#pragma once
#include "../Shader.h"
#include <vector>
#include "SceneObject.h"
#include "Light.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Scene {
public:
    Scene();
    ~Scene();

    void AddObject(SceneObject* object);
    void RemoveObject(SceneObject* object);
    void AddLight(Light* light);
    void RemoveLight(Light* light);
    void Update(float deltaTime);
    void Render(Shader *shader);
    void SetCameraView(glm::vec3 position, glm::vec3 target, glm::vec3 up);
    void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
    void AdjustCameraZoom(float zoomDelta);
    void RotateCamera(float yaw, float pitch);
    void MoveCamera(const glm::vec3& direction);
    std::vector<SceneObject*> GetSceneNodes();
    std::vector<Light*> GetLights() const { return lights; }
    
    // Mesh picking related methods
    void SetactiveMesh(SceneObject* mesh);
    SceneObject* GetActiveMesh() const { return activeMesh; }
    SceneObject* PickMesh(const glm::vec3& rayOrigin, const glm::vec3& rayDir);
    glm::mat4 GetViewMatrix() const { return viewMatrix; }
    glm::mat4 GetProjectionMatrix() const { return projectionMatrix; }
    glm::vec3 GetCameraPosition() const { return cameraPosition; }
    glm::vec3 GetCameraTarget() const { return cameraTarget; }

private:
    std::vector<SceneObject*> objects;
    std::vector<Light*> lights;
    SceneObject* activeMesh;  // Currently selected mesh
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUp;
    float zoomLevel;
    float yaw;   // Camera rotation around Y axis
    float pitch; // Camera rotation around X axis
    void UpdateCameraVectors();
};
