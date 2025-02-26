#include "Scene/Scene.h"

Scene::Scene(){

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

void Scene::Update(float deltaTime) {
    for (SceneObject* obj : objects) {
        obj->Update(deltaTime);
    }
}

void Scene::Render() {
    for (SceneObject* obj : objects) {
        obj->Render();
    }
}