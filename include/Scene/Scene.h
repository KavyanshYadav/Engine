#pragma once
#include "../Shader.h"
#include <vector>
#include "SceneObject.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Scene {
public:
    Scene();
    ~Scene();

    void AddObject(SceneObject* object);
    void RemoveObject(SceneObject* object);
    void Update(float deltaTime);
    void Render(Shader *shader);
    void SetCameraView(glm::vec3 position, glm::vec3 target, glm::vec3 up);
    void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
    std::vector<SceneObject*> GetSceneNodes ();

private:
    std::vector<SceneObject*> objects;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};
