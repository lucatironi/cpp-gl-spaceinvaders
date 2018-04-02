#include <iostream>

#include "projectile_manager.hpp"

ProjectileManager::ProjectileManager(GLuint amount)
    : amount(amount)
{
    // Create this->amount default projectile instances
    for (GLuint i = 0; i < this->amount; ++i)
    {
        this->lasers.push_back(Projectile(glm::vec3(0.0f, 1.0f, 0.0f)));
        this->bombs.push_back(Projectile(glm::vec3(1.0f, 1.0f, 1.0f)));
    }
}

ProjectileManager::~ProjectileManager()
{
}

void ProjectileManager::FireLaser(glm::vec2 from, glm::vec2 velocity)
{
    Projectile *projectile = this->firstUnusedLaser();

    projectile->Position = from;
    projectile->Velocity = velocity;
    projectile->Life = 1.0f;
}

void ProjectileManager::FireBomb(glm::vec2 from, glm::vec2 velocity)
{
    Projectile *projectile = this->firstUnusedBomb();

    projectile->Position = from;
    projectile->Velocity = velocity;
    projectile->Life = 1.0f;
}

void ProjectileManager::Update(GLfloat deltaTime, GLuint windowHeight)
{
    for (Projectile &laser : this->lasers)
    {
        if (laser.Life > 0.0f)
            laser.Position += laser.Velocity * deltaTime;
        if (laser.Position.y <= 0)
            laser.Life = 0.0f;
    }

    for (Projectile &bomb : this->bombs)
    {
        if (bomb.Life > 0.0f)
            bomb.Position += bomb.Velocity * deltaTime;
        if (bomb.Position.y >= windowHeight)
            bomb.Life = 0.0f;
    }
}

void ProjectileManager::Draw(SpriteRenderer &renderer)
{
    for (Projectile laser : this->lasers)
    {
        if (laser.Life > 0.0f)
            laser.Draw(renderer);
    }

    for (Projectile bomb : this->bombs)
    {
        if (bomb.Life > 0.0f)
            bomb.Draw(renderer);
    }
}

// Stores the index of the last projectile used (for quick access to next dead projectile)
GLuint lastUsedLaser = 0;
Projectile *ProjectileManager::firstUnusedLaser()
{
    // First search from last used projectile, this will usually return almost instantly
    for (GLuint i = lastUsedLaser; i < this->amount; ++i)
    {
        if (this->lasers[i].Life <= 0.0f)
        {
            lastUsedLaser = i;
            return &this->lasers[i];
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedLaser; ++i)
    {
        if (this->lasers[i].Life <= 0.0f)
        {
            lastUsedLaser = i;
            return &this->lasers[i];
        }
    }
    // All projectiles are taken, override the first one (note that if it repeatedly hits this case, more projectiles should be reserved)
    lastUsedLaser = 0;
    return &this->lasers[0];
}

// Stores the index of the last projectile used (for quick access to next dead projectile)
GLuint lastUsedBomb = 0;
Projectile *ProjectileManager::firstUnusedBomb()
{
    // First search from last used projectile, this will usually return almost instantly
    for (GLuint i = lastUsedBomb; i < this->amount; ++i)
    {
        if (this->bombs[i].Life <= 0.0f)
        {
            lastUsedBomb = i;
            return &this->bombs[i];
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedBomb; ++i)
    {
        if (this->bombs[i].Life <= 0.0f)
        {
            lastUsedBomb = i;
            return &this->bombs[i];
        }
    }
    // All projectiles are taken, override the first one (note that if it repeatedly hits this case, more projectiles should be reserved)
    lastUsedBomb = 0;
    return &this->bombs[0];
}