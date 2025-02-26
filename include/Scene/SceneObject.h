#pragma once 


class SceneObject {

    public:
        virtual ~SceneObject() = default;
        virtual void Update(float deltaTime) = 0;
        virtual void Render() = 0;
    };
