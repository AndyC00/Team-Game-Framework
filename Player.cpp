#include "Player.h"
#include "entity.h"
#include "renderer.h"
#include "Projectile.h"
#include <iostream>

const float PLAYER_MOVE_SPEED = 210.0f;

Player::Player()
    : Entity(),
    m_facingDirection(1, 0),
    m_moveSpeed(PLAYER_MOVE_SPEED),
    m_lives(3),
    m_currentWeapon(1),
    m_attackCooldown(0.3f),
    m_attackCooldownRemaining(0.0f),
    m_pFmodSystem(nullptr),
    m_pMeleeSound(nullptr),
    m_pShootSound(nullptr)
{
    // Initialize FMOD system
    FMOD::System_Create(&m_pFmodSystem);
    m_pFmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);

    // Load shoot sound
    m_pFmodSystem->createSound("sounds\\Player_melee.mp3", FMOD_DEFAULT, 0, &m_pMeleeSound);
    m_pFmodSystem->createSound("sounds\\Player_gunshot.mp3", FMOD_DEFAULT, 0, &m_pShootSound);
}

Player::~Player()
{
    for (auto projectile : m_projectiles)
    {
        delete projectile;  // Clean up projectiles when player is destroyed
    }

    for (auto melee : m_melee)
    {
        delete melee;
    }

    m_pShootSound->release();
    m_pMeleeSound->release();
    m_pFmodSystem->close();
    m_pFmodSystem->release();
}

bool Player::Initialise(Renderer& renderer)
{
    m_pSprite = renderer.CreateSprite("Sprites\\Hunter.png");
    m_pSprite->SetScale(0.1f);
    return true;
}


void Player::Process(float deltaTime, InputSystem& inputSystem, Renderer& renderer)
{
    // Initialize the movement vector
    Vector2 movement(0.0f, 0.0f);
    // When melee attack the player stand still
    if (!(m_currentWeapon == 1 && m_attackCooldownRemaining > 0.0f))
    {
        // Check for movement input and accumulate movement in respective directions
        if (inputSystem.GetKeyState(SDL_SCANCODE_UP) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_UP) == BS_HELD)
        {
            // Move up
            movement.y -= 1.0f;
            // Face up
            m_facingDirection.Set(0.0f, -1.0f);
        }
        if (inputSystem.GetKeyState(SDL_SCANCODE_DOWN) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_DOWN) == BS_HELD)
        {
            // Move down
            movement.y += 1.0f;
            // Face down
            m_facingDirection.Set(0.0f, 1.0f);
        }
        if (inputSystem.GetKeyState(SDL_SCANCODE_LEFT) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_LEFT) == BS_HELD)
        {
            // Move left
            movement.x -= 1.0f;
            // Face left
            m_facingDirection.Set(-1.0f, 0.0f);
           
        }
        if (inputSystem.GetKeyState(SDL_SCANCODE_RIGHT) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_RIGHT) == BS_HELD)
        {
            // Move right
            movement.x += 1.0f;
            // Face right
            m_facingDirection.Set(1.0f, 0.0f);
        }
    }

    // Update the position based on the movement vector and speed
    m_position += movement * m_moveSpeed * deltaTime;

    // Get screen boundaries from the renderer
    float screenWidth = static_cast<float>(renderer.GetWidth());
    float screenHeight = static_cast<float>(renderer.GetHeight());

    // Define player's sprite width and height for boundary checking
    float spriteWidth = m_pSprite->GetWidth() * m_pSprite->GetScale();
    float spriteHeight = m_pSprite->GetHeight() * m_pSprite->GetScale();

    // Boundary check to keep the player within the screen
    if (m_position.x < 0.0f)
    {
        m_position.x = 0.0f;
    }

    if (m_position.x + spriteWidth > screenWidth)
    {
        m_position.x = screenWidth - spriteWidth;
    }

    if (m_position.y < 0.0f)
    {
        m_position.y = 0.0f;
    }

    if (m_position.y + spriteHeight > screenHeight)
    {
        m_position.y = screenHeight - spriteHeight;
    }

    // Weapon switching
    if (inputSystem.GetKeyState(SDL_SCANCODE_1) == BS_PRESSED)
    {
        // Switch to Melee
        m_currentWeapon = 1;
    }
    else if (inputSystem.GetKeyState(SDL_SCANCODE_2) == BS_PRESSED)
    {
        // Switch to Projectile
        m_currentWeapon = 2;
    }

    // Handle attack cooldown
    m_attackCooldownRemaining -= deltaTime;
    if (m_attackCooldownRemaining <= 0.0f)
    {
        if (inputSystem.GetKeyState(SDL_SCANCODE_SPACE) == BS_PRESSED)
        {
            Attack(renderer);
            m_attackCooldownRemaining = m_attackCooldown;
        }
    }

    // Update all projectiles
    for (auto it = m_projectiles.begin(); it != m_projectiles.end();)
    {
        (*it)->Process(deltaTime);
        // Remove "dead" projectiles
        if (!(*it)->IsAlive())
        {
            delete* it;
            // Erase the projectile
            it = m_projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Update all melee hitboxes
    for (auto it = m_melee.begin(); it != m_melee.end();)
    {
        (*it)->Process(deltaTime);

        // Remove "dead" melee hitboxes
        if (!(*it)->IsAlive())
        {
            delete* it;
            // Erase the hitbox
            it = m_melee.erase(it);
        }
        else
        {
            ++it;
        }
    }

    Entity::Process(deltaTime);
}


void Player::Attack(Renderer& renderer)
{
    // Melee attack
    if (m_currentWeapon == 1)
    {
        std::cout << "Melee attack in direction: (" << m_facingDirection.x << ", " << m_facingDirection.y << ")" << std::endl;

        // Create a new melee hitbox
        MeleeHitbox* newMeleeHitbox = new MeleeHitbox();
        if (newMeleeHitbox->Initialise(renderer, m_position, m_facingDirection))
        {
            
            m_melee.push_back(newMeleeHitbox);
            m_pFmodSystem->playSound(m_pMeleeSound, 0, false, nullptr);
            m_pFmodSystem->update();
        }
    }
    // Projectile attack
    else if (m_currentWeapon == 2)
    {
        std::cout << "Shooting projectile in direction: (" << m_facingDirection.x << ", " << m_facingDirection.y << ")" << std::endl;

        // Create a new projectile and store it
        Projectile* newProjectile = new Projectile();
        if (newProjectile->Initialise(renderer, m_position, m_facingDirection))
        {
            m_projectiles.push_back(newProjectile);
            // Play shooting sound
            m_pFmodSystem->playSound(m_pShootSound, 0, false, nullptr);
            m_pFmodSystem->update();
        }
    }
}

void Player::Draw(Renderer& renderer)
{
    // Draw the player
    Entity::Draw(renderer);

    // Draw all projectiles
    for (auto projectile : m_projectiles)
    {
        projectile->Draw(renderer);
    }

    // Draw all melee hitboxes
    for (auto meleeHitbox : m_melee)
    {
        meleeHitbox->Draw(renderer);
    }
}