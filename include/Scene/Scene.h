#pragma once
#include <vector>
#include "SceneObject.h"

class Scene {
public:
    Scene();
    ~Scene();

    void AddObject(SceneObject* object);
    void RemoveObject(SceneObject* object);
    void Update(float deltaTime);
    void Render();

private:
    std::vector<SceneObject*> objects;
};
