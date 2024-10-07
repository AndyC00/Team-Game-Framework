#ifndef _MELEEHITBOX_H_
#define _MELEEHITBOX_H_

#include "Entity.h"
#include "Vector2.h"
#include "renderer.h"

class MeleeHitbox : public Entity
{
public:
    MeleeHitbox();
    ~MeleeHitbox();

    bool Initialise(Renderer& renderer, const Vector2& position, const Vector2& direction);
    void Process(float deltaTime);
    void Draw(Renderer& renderer);

    bool IsAlive() const;  // Check if the melee hitbox is still active

private:
    float m_lifespan;  // Duration the melee hitbox will be active
    float m_elapsedTime;  // Time since the melee hitbox was created
    int m_damage;  // Damage dealt by the melee attack
};

#endif // _MELEEHITBOX_H_
