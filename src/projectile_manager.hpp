#ifndef PROJECTILE_MANAGER_H
#define PROJECTILE_MANAGER_H
#include <vector>

#include <glm/glm.hpp>

#include "game_object.hpp"

class Projectile : public GameObject
{
    public:
        Projectile()
          : GameObject(glm::vec2(0.0f, 0.0f),
                       glm::vec2(1.0f, 8.0f),
                       glm::vec3(1.0f, 1.0f, 1.0f),
                       glm::vec2(0.0f, 0.0f)),
            Life(0.0f){};
        Projectile(glm::vec2 position, glm::vec2 velocity, GLfloat life)
          : GameObject(position,
                       glm::vec2(1.0f, 8.0f),
                       glm::vec3(1.0f, 1.0f, 1.0f),
                       velocity),
            Life(life){};

        Projectile(glm::vec3 color)
            : GameObject(glm::vec2(0.0f, 0.0f),
                         glm::vec2(1.0f, 8.0f),
                         color,
                         glm::vec2(0.0f, 0.0f)),
              Life(0.0f){};

        GLfloat Life;
};

class ProjectileManager
{
    public:
        ProjectileManager(GLuint amount);
        ~ProjectileManager();

        void FireLaser(glm::vec2 from, glm::vec2 velocity);

        void FireBomb(glm::vec2 from, glm::vec2 velocity);

        void Update(GLfloat deltaTime, GLuint windowHeight);
        void Draw(SpriteRenderer &renderer);

        std::vector<Projectile> lasers;
        std::vector<Projectile> bombs;

      private:
        GLuint amount;

        Projectile *firstUnusedLaser();
        Projectile *firstUnusedBomb();
};
#endif