#include "MeleeHitbox.h"
#include "sprite.h"
#include <iostream>

const float MELEE_LIFESPAN = 0.2f;  // Duration for which the hitbox exists

MeleeHitbox::MeleeHitbox()
    : m_lifespan(MELEE_LIFESPAN), m_elapsedTime(0.0f), m_damage(10)
{
}

MeleeHitbox::~MeleeHitbox()
{
}

bool MeleeHitbox::Initialise(Renderer& renderer, const Vector2& position, const Vector2& direction)
{
    m_position = position + direction * 15.0f;
    m_velocity = direction * 100.0f;  // Move in the direction the player is facing
    m_pSprite = renderer.CreateSprite("Sprites\\slash.png");
    if (direction.x == 1 && direction.y == 0)
    {
        m_pSprite->SetScale(-0.2f);
        m_pSprite->SetAngle(90.0f);
    }
    else if (direction.x == 0 && direction.y == -1)
    {
        m_pSprite->SetScale(-0.2f);
    }
    else
    {
        m_pSprite->SetScale(0.2f);  // Scale the hitbox sprite
    }
    return true;
}

void MeleeHitbox::Process(float deltaTime)
{
    m_elapsedTime += deltaTime;

    if (m_elapsedTime >= m_lifespan)
    {
        m_bAlive = false;  // Mark hitbox as dead when its lifespan ends
    }

    Entity::Process(deltaTime);  // Update position
}

void MeleeHitbox::Draw(Renderer& renderer)
{
    Entity::Draw(renderer);  // Draw the hitbox
}

bool MeleeHitbox::IsAlive() const
{
    return m_bAlive;
}
