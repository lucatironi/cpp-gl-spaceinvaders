#include "game_object.hpp"

GameObject::GameObject()
    : Position(0, 0), Size(1, 1), Color(1.0f), Velocity(0.0f), Rotation(0.0f) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Color(color), Velocity(velocity), Rotation(0.0f) {}

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Position, this->Size, this->Rotation, this->Color);
}