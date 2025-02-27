#include "Scene/Scene.h"

Scene::Scene(){
    SetCameraView(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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