#include "Projectile.h"
#include "renderer.h"
#include <iostream>
#include "sprite.h"

const float PROJECTILE_SPEED = 400.0f;
const float PROJECTILE_LIFESPAN = 3.0f;  // 3 seconds lifespan

Projectile::Projectile()
    : m_speed(PROJECTILE_SPEED),
    m_lifespan(PROJECTILE_LIFESPAN),
    m_damage(10)
{
}

Projectile::~Projectile()
{
}

bool Projectile::Initialise(Renderer& renderer, const Vector2& startPosition, const Vector2& direction)
{
    m_pSprite = renderer.CreateSprite("Sprites\\ball.png");
    m_pSprite->SetScale(0.05f);
    m_position = startPosition;
    m_direction = direction;
    m_direction.Normalise();  // Ensure the direction is normalized
    m_pSprite->SetGreenTint(0.0f);
    m_pSprite->SetBlueTint(0.0f);
    m_pSprite->SetRedTint(0.0f);
    return true;
}

void Projectile::Process(float deltaTime)
{
    // Move the projectile
    m_position += m_direction * m_speed * deltaTime;

    // Update sprite position
    m_pSprite->SetX(static_cast<int>(m_position.x));
    m_pSprite->SetY(static_cast<int>(m_position.y));

    // Reduce lifespan
    m_lifespan -= deltaTime;

    if (m_lifespan <= 0.0f)
    {
        m_bAlive = false;  // Mark projectile as dead when its lifespan is over
    }
}

int Projectile::GetDmg()const
{
    return m_damage;
}