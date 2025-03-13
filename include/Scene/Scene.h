#pragma once
#include "../Shader.h"
#include <vector>
#include "SceneObject.h"
#include "Light.h"
#include "Gizmo.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

// Debug state structure
struct DebugState {
    bool showBoundingBoxes = false;
    bool wireframeMode = false;
    bool showNormals = false;
    bool showGrid = true;
    bool showVertexPoints = false;
};

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

    // Gizmo related methods
    void ShowGizmo(bool show) { gizmo->SetVisible(show); }
    bool IsGizmoVisible() const { return gizmo->IsVisible(); }
    void SetGizmoScale(float scale) { gizmo->SetScale(scale); }

    // Debug state management
    static DebugState& GetDebugState() { return debugState; }
    static Shader* GetBoundingBoxShader() { return boundingBoxShader; }

private:
    std::vector<SceneObject*> objects;
    std::vector<Light*> lights;
    SceneObject* activeMesh;  // Currently selected mesh
    Gizmo* gizmo;            // Transformation gizmo
    Shader* gizmoShader;     // Shader for the gizmo
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUp;
    float zoomLevel;
    float yaw;   // Camera rotation around Y axis
    float pitch; // Camera rotation around X axis
    void UpdateCameraVectors();

    static DebugState debugState;
    static Shader* boundingBoxShader;
};
