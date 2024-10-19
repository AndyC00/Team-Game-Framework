#include "Player.h"
#include "entity.h"
#include "renderer.h"
#include "Projectile.h"

#include <iostream>

const float PLAYER_MOVE_SPEED = 210.0f;
const float INVINCIBILITY_DURATION = 1.0f;
const int TILE_SIZE = 64;

Player::Player()
    : Entity(),
    m_facingDirection(1, 0),
    m_moveSpeed(PLAYER_MOVE_SPEED),
    m_lives(5),
    m_currentWeapon(1),
    m_attackCooldown(0.3f),
    m_attackCooldownRemaining(0.0f),
    m_pFmodSystem(nullptr),
    m_pMeleeSound(nullptr),
    m_pShootSound(nullptr),
    m_invincibilityRemaining(0.0f),
    m_bAlive(true)
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
    m_projectiles.clear();

    for (auto melee : m_melee)
    {
        delete melee;
    }
    m_melee.clear();

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


void Player::Process(float deltaTime, InputSystem& inputSystem, Renderer& renderer, DungeonRoom& dungeonRoom)
{
    // Update invincibility timer
    if (m_invincibilityRemaining > 0.0f)
    {
        m_invincibilityRemaining -= deltaTime;
    }

    // Initialize the movement vector
    Vector2 movement(0.0f, 0.0f);

    // When melee attack, the player stands still
    if (!(m_currentWeapon == 1 && m_attackCooldownRemaining > 0.0f))
    {
        // Check for movement input and accumulate movement in respective directions
        if (inputSystem.GetKeyState(SDL_SCANCODE_UP) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_UP) == BS_HELD)
        {
            movement.y -= 1.0f;  // Move up
            m_facingDirection.Set(0.0f, -1.0f);  // Face up
        }
        if (inputSystem.GetKeyState(SDL_SCANCODE_DOWN) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_DOWN) == BS_HELD)
        {
            movement.y += 1.0f;  // Move down
            m_facingDirection.Set(0.0f, 1.0f);  // Face down
        }
        if (inputSystem.GetKeyState(SDL_SCANCODE_LEFT) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_LEFT) == BS_HELD)
        {
            movement.x -= 1.0f;  // Move left
            m_facingDirection.Set(-1.0f, 0.0f);  // Face left
        }
        if (inputSystem.GetKeyState(SDL_SCANCODE_RIGHT) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_RIGHT) == BS_HELD)
        {
            movement.x += 1.0f;  // Move right
            m_facingDirection.Set(1.0f, 0.0f);  // Face right
        }
    }

    // Calculate new position based on the movement vector and speed
    Vector2 newPosition = m_position + (movement * m_moveSpeed * deltaTime);

    // Convert new position to tile coordinates for collision checking
    int tileX = static_cast<int>(newPosition.x / TILE_SIZE);
    int tileY = static_cast<int>(newPosition.y / TILE_SIZE);

    // Check if the new tile is passable
    if (dungeonRoom.IsTilePassable(tileX, tileY))
    {
        // Only update position if the tile is passable
        m_position = newPosition;
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
        if (!(*it)->IsAlive())
        {
            delete* it;
            it = m_projectiles.erase(it);  // Remove "dead" projectiles
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
        if (!(*it)->IsAlive())
        {
            delete* it;
            it = m_melee.erase(it);  // Remove "dead" melee hitboxes
        }
        else
        {
            ++it;
        }
    }

    // Call base Entity process (if needed)
    Entity::Process(deltaTime);
}



void Player::Attack(Renderer& renderer)
{
    // Melee attack
    if (m_currentWeapon == 1)
    {
        std::cout << "Melee attack in direction: (" << m_facingDirection.x << ", " << m_facingDirection.y << ")" << std::endl;

        // Create a new melee hitbox
        auto meleeHitbox = new MeleeHitbox();
        if (meleeHitbox->Initialise(renderer, m_position, m_facingDirection))
        {
            m_melee.push_back(meleeHitbox);
            m_pFmodSystem->playSound(m_pMeleeSound, 0, false, nullptr);
            m_pFmodSystem->update();
        }
        else
        {
            delete meleeHitbox;
        }
    }
    // Projectile attack
    else if (m_currentWeapon == 2)
    {
        std::cout << "Shooting projectile in direction: (" << m_facingDirection.x << ", " << m_facingDirection.y << ")" << std::endl;

        // Create a new projectile and store it
        auto projectile = new Projectile();
        if (projectile->Initialise(renderer, m_position, m_facingDirection))
        {
            m_projectiles.push_back(projectile);
            // Play shooting sound
            m_pFmodSystem->playSound(m_pShootSound, 0, false, nullptr);
            m_pFmodSystem->update();
        }
        else
        {
            delete projectile;
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

int Player::GetLives() const
{
    return m_lives;
}

void Player::TakeDamage(int damage)
{
    if (m_invincibilityRemaining <= 0.0f)  // Check if player is invincible
    {
        m_lives -= damage;
        // Set the invincibility timer
        m_invincibilityRemaining = INVINCIBILITY_DURATION;
    }
}


int Player::GetWeapons() const
{
    return m_currentWeapon;
}

void Player::SetDead()
{
    m_bAlive = false;
}

std::vector<MeleeHitbox*>& Player::GetMeleeHitboxes()
{
    return m_melee;
}

std::vector<Projectile*>& Player::GetProjectiles()
{
    return m_projectiles;
}