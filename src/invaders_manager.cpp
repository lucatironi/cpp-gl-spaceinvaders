#include "invaders_manager.hpp"

InvadersManager::InvadersManager(GLuint amount, GLuint rowAmount)
    : amount(amount), rowAmount(rowAmount)
{
    this->Init();
}

InvadersManager::~InvadersManager()
{
}

glm::vec2 velocity(50.0f, 0.0f);
GLfloat slideTime = 0.0f;
GLboolean bounceAndSlide = false;

void InvadersManager::Update(GLfloat deltaTime, GLuint windowWidth, GLuint windowHeight)
{

    for (Invader &invader : this->fleet)
        invader.Position += velocity * deltaTime;

    if (this->fleet[0].Position.x < 8.0f)
    {
        this->fleet[0].Position.x = 8.0f;
        for (Invader &invader : this->fleet)
            invader.Position.y += 16.0f;

        velocity.x *= -1.0f;
    }
    if (this->fleet[rowAmount - 1].Position.x > windowWidth - INVADER_SIZE.x - 8.0f)
    {
        this->fleet[rowAmount - 1].Position.x = windowWidth - INVADER_SIZE.x - 8.0;
        for (Invader &invader : this->fleet)
            invader.Position.y += 16.0f;

        velocity.x *= -1.0f;
    }
}

void InvadersManager::Draw(SpriteRenderer &renderer)
{
    for (Invader invader : this->fleet)
    {
        if (!invader.Destroyed)
            invader.Draw(renderer);
    }
}

void InvadersManager::Init()
{
    this->fleet.clear();
    for (GLuint count = 0, x = 0, y = 0, i = 0, j = 0; count < this->amount; ++count, ++i)
    {
        if (count % this->rowAmount == 0)
        {
            i = 0;
            j++;
        }
        x = INVADER_SIZE.x * 2 * i + 16.0f;
        y = INVADER_SIZE.y * 2 * j + 16.0f;

        this->fleet.push_back(Invader(glm::vec2(x, y), INVADER_SIZE, glm::vec3(1.0f), glm::vec2(0.0f)));
    }
}

GLboolean InvadersManager::AllDead()
{
    if (std::all_of(this->fleet.begin(), this->fleet.end(), [](Invader i) { return i.Destroyed == GL_TRUE; }))
        return GL_TRUE;
    else
        return GL_FALSE;
}
GLboolean InvadersManager::ReachedBoundary(GLuint boundary)
{
    if (std::any_of(this->fleet.begin(), this->fleet.end(), [&](Invader i) { return i.Destroyed == GL_FALSE && i.Position.y > boundary; }))
        return GL_TRUE;
    else
        return GL_FALSE;
}
