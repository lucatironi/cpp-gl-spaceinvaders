#include <sstream>

#include "game.hpp"
#include "resource_manager.hpp"

Game::Game(GLuint windowWidth, GLuint windowHeight, GLuint framebufferWidth, GLuint framebufferHeight)
    : State(GAME_MENU),
      Keys(),
      KeysProcessed(),
      WindowWidth(windowWidth),
      WindowHeight(windowHeight),
      FramebufferWidth(framebufferWidth),
      FramebufferHeight(framebufferHeight),
      PlayerLives(3),
      PlayerScore(0)
{
}

Game::~Game()
{
    delete this->Renderer;
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("../src/shaders/sprite.vs", "../src/shaders/sprite.fs", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->WindowWidth), static_cast<GLfloat>(this->WindowHeight), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
    // Set render-specific controls
    this->Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    this->Projectiles = new ProjectileManager(25);
    this->Invaders = new InvadersManager(55, 11);
    this->Text = new TextRenderer(this->WindowWidth, this->WindowHeight);
    this->Text->Load("../assets/PressStart2P-Regular.ttf", 16);

    // Initalize game objects
    this->InitPlayer();

    GLfloat offset = 0;
    for (GLuint i = 0; i < 4; ++i)
    {
        offset += 146.0f;
        glm::vec2 barrierPosition(offset, this->WindowHeight - 64.0f - LASERCANNON_SIZE.y - SCREEN_PADDING * 2);
        this->Barriers.push_back(GameObject(barrierPosition, glm::vec2(64.0f, 32.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    }
}

void Game::ProcessInput(GLfloat deltaTime)
{
    if (this->State == GAME_MENU || this->State == GAME_WIN || this->State == GAME_LOST)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            this->Reset();
            this->State = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
        }
    }
    if (this->State == GAME_ACTIVE)
    {
        GLfloat deltaSpace = LASERCANNON_VELOCITY * deltaTime;
        // Move player laser cannon
        if (this->Keys[GLFW_KEY_A]) // Left
        {
            if (this->PlayerLaserCannon->Position.x >= SCREEN_PADDING)
                this->PlayerLaserCannon->Position.x -= deltaSpace;
        }
        if (this->Keys[GLFW_KEY_D]) // Right
        {
            if (this->PlayerLaserCannon->Position.x <= this->WindowWidth - this->PlayerLaserCannon->Size.x - SCREEN_PADDING)
                this->PlayerLaserCannon->Position.x += deltaSpace;
        }
        // Fire lasers
        if (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE])
        {
            // Add projectile to list
            glm::vec2 laserSpawnPoint = glm::vec2(
                this->PlayerLaserCannon->Position.x + LASERCANNON_SIZE.x / 2,
                this->PlayerLaserCannon->Position.y
            );
            this->Projectiles->FireLaser(laserSpawnPoint, LASER_VELOCITY); // Up!
            this->KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
        }
    }
}

void Game::Update(GLfloat deltaTime)
{
    if (this->State == GAME_ACTIVE)
    {
        this->Projectiles->Update(deltaTime, this->WindowHeight);
        this->Invaders->Update(deltaTime, this->WindowWidth, this->WindowHeight);
        this->SpawnBombs();
        this->DoCollisions();
    }
    if (this->Invaders->AllDead())
        this->State = GAME_WIN;

    if (this->PlayerLives == 0 ||
        this->Invaders->fleet[54].Position.y > this->WindowHeight - 64.0f - LASERCANNON_SIZE.y - SCREEN_PADDING * 2)
        this->State = GAME_LOST;
}

void Game::Render(GLfloat deltaTime)
{
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN)
    {
        this->PlayerLaserCannon->Draw(*Renderer);
        this->Projectiles->Draw(*Renderer);
        this->Invaders->Draw(*Renderer);
        for (GameObject &barrier : this->Barriers)
            barrier.Draw(*Renderer);
    }

    if (this->State == GAME_MENU)
        Text->RenderText("Press ENTER to start", 250.0f, this->WindowHeight / 2, 1.0f);

    if (this->State == GAME_WIN)
        Text->RenderText("You WON!!!", 320.0f, this->WindowHeight / 2 - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    if (this->State == GAME_LOST)
        Text->RenderText("You LOST...", 320.0f, this->WindowHeight / 2 - 20.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    if (this->State == GAME_WIN || this->State == GAME_LOST)
        Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->WindowHeight / 2, 1.0f);

    // Render Lives and Score
    std::stringstream lives;
    lives << "Lives: " << this->PlayerLives;
    Text->RenderText(lives.str(), 8.0f, 8.0f, 1.0f);

    std::stringstream score;
    score << "Score: " << this->PlayerScore;
    Text->RenderText(score.str(), this->WindowWidth - 200.0f, 8.0f, 1.0f);

    // Render FPS
    std::stringstream fps;
    fps << (int)(1 / deltaTime);
    Text->RenderText(fps.str(), this->WindowWidth - 30.0f, this->WindowHeight - 20.0f, 0.5f);
}

void Game::Reset()
{
    this->InitPlayer();
    this->Invaders->Init();
    this->Projectiles->Init();
}

void Game::InitPlayer()
{
    this->PlayerLives = 3;
    this->PlayerScore = 0;

    glm::vec2 playerPosition = glm::vec2(
        this->WindowWidth / 2 - LASERCANNON_SIZE.x / 2,
        this->WindowHeight - LASERCANNON_SIZE.y - SCREEN_PADDING);
    this->PlayerLaserCannon = new GameObject(playerPosition, LASERCANNON_SIZE, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f));
}

GLboolean ShouldSpawn(GLuint chance)
{
    GLuint random = rand() % chance;
    return random == 0;
}

void Game::SpawnBombs()
{
    for (Invader &invader : Invaders->fleet)
    {
        if (!invader.Destroyed && ShouldSpawn(BOMB_SPAWN_CHANCE))
        {
            glm::vec2 bombSpawnPoint = glm::vec2(
                invader.Position.x + INVADER_SIZE.x / 2,
                invader.Position.y + INVADER_SIZE.y);
            this->Projectiles->FireBomb(bombSpawnPoint, BOMB_VELOCITY); // Up!
        }
    }
}

// Collision detection
GLboolean CheckCollision(GameObject &one, GameObject &two);

void Game::DoCollisions()
{
    for (Invader &invader : Invaders->fleet)
    {
        if (!invader.Destroyed)
        {
            for (Projectile &laser : Projectiles->lasers)
            {
                if (laser.Life > 0.0f && CheckCollision(laser, invader))
                {
                    this->PlayerScore += 80;
                    invader.Destroyed = GL_TRUE;
                    laser.Life = 0.0f;
                }
            }
        }
    }
    for (Projectile &bomb : Projectiles->bombs)
    {
        if (bomb.Life > 0.0f && CheckCollision(bomb, *this->PlayerLaserCannon))
        {
            this->PlayerLives--;
            bomb.Life = 0.0f;
        }
        for (GameObject &barrier : this->Barriers)
        {
            if (bomb.Life > 0.0f && CheckCollision(bomb, barrier))
            {
                bomb.Life = 0.0f;
            }
        }
    }

    for (Projectile &laser : Projectiles->lasers)
    {
        for (GameObject &barrier : this->Barriers)
        {
            if (laser.Life > 0.0f && CheckCollision(laser, barrier))
            {
                laser.Life = 0.0f;
            }
        }
    }
}

GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    // Collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}