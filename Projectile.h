#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "Entity.h"
#include "Vector2.h"

class Projectile : public Entity
{
public:
    Projectile();
    ~Projectile();

    bool Initialise(Renderer& renderer, const Vector2& startPosition, const Vector2& direction);
    void Process(float deltaTime);
    int GetDmg() const;

private:
    Vector2 m_direction;  // Direction of projectile movement
    float m_speed;
    float m_lifespan;  // Time before the projectile disappears
    int m_damage;  // Damage dealt by the projectile
};

#endif // _PROJECTILE_H_
