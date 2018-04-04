#ifndef INVADERS_MANAGER_H
#define INVADERS_MANAGER_H
#include <vector>

#include <glm/glm.hpp>

#include "game_object.hpp"

const glm::vec2 INVADER_SIZE(32, 24);

class Invader : public GameObject
{
    public:
        Invader()
          : GameObject(),
            Destroyed(false){};

        Invader(glm::vec2 position, glm::vec2 size, glm::vec3 color, glm::vec2 velocity)
          : GameObject(position, size, color, velocity),
            Destroyed(false){};

      GLboolean Destroyed;
};

class InvadersManager
{
    public:
        InvadersManager(GLuint amount, GLuint rowAmount);
        ~InvadersManager();

        void Update(GLfloat deltaTime, GLuint windowWidth, GLuint windowHeight);
        void Draw(SpriteRenderer &renderer);
        void Init();

        std::vector<Invader> fleet;

    private:
        GLuint amount, rowAmount;
};
#endif