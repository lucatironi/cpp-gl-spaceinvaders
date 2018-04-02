#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

#include "sprite_renderer.hpp"

class GameObject
{
    public:
        glm::vec2 Position, Size;
        glm::vec3 Color;
        glm::vec2 Velocity;
        GLfloat Rotation;

        GameObject();
        GameObject(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
        
        virtual void Draw(SpriteRenderer &renderer);
};

#endif