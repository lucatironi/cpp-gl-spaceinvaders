#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "sprite_renderer.hpp"
#include "projectile_manager.hpp"
#include "invaders_manager.hpp"
#include "game_object.hpp"
#include "text_renderer.hpp"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOST
};

const glm::vec2 LASERCANNON_SIZE(32, 16);
const GLfloat   LASERCANNON_VELOCITY(500.0f);
const glm::vec2 LASER_VELOCITY(0.0f, -450.0f);
const glm::vec2 BOMB_VELOCITY(0.0f, 300.0f);
const GLfloat   SCREEN_PADDING(16.0f);
const GLuint    BOMB_SPAWN_CHANCE(1500);
const GLuint    INVADERS_COUNT(55);
const GLuint    INVADERS_COLUMNS(11);

class Game
{
    public:
        GameState State;
        GLboolean Keys[1024];
        GLboolean KeysProcessed[1024];

        Game(GLuint windowWidth, GLuint windowHeight, GLuint framebufferWidth, GLuint framebufferHeight);
        ~Game();

        void Init();
        void ProcessInput(GLfloat deltaTime);
        void Update(GLfloat deltaTime);
        void Render(GLfloat deltaTime);
        void DoCollisions();
        void SpawnBombs();

        void InitPlayer();
        void Reset();

        void SetFramebufferSize(GLuint framebufferWidth, GLuint framebufferHeight)
            { this->FramebufferWidth = framebufferWidth; this->FramebufferHeight = framebufferHeight; };

    private:
        GLuint WindowWidth, WindowHeight, FramebufferWidth, FramebufferHeight;

        SpriteRenderer          *Renderer;
        TextRenderer            *Text;
        ProjectileManager       *Projectiles;
        InvadersManager         *Invaders;
        GameObject              *PlayerLaserCannon;
        std::vector<GameObject> Barriers;
        GLuint                  PlayerLives;
        GLuint                  PlayerScore;
};

#endif