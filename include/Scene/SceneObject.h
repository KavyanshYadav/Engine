#pragma once 
#include "../Shader.h"

class SceneObject {

    public:
        virtual ~SceneObject() = default;
        virtual void Update(float deltaTime) = 0;
        virtual void Render(Shader* shader) = 0;
    };
