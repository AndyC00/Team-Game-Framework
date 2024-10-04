#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Entity.h"
#include "InputSystem.h"
#include "Vector2.h"
#include "Projectile.h"
#include <vector>  // To store multiple projectiles
#include "sprite.h"
#include "MeleeHitbox.h"

class Sprite;

class Player : public Entity
{
public:
    Player();
    ~Player();

    // Member methods:
    bool Initialise(Renderer& renderer);
    void Process(float deltaTime, InputSystem& inputSystem, Renderer& renderer);
    void Attack(Renderer& renderer);
    void Draw(Renderer& renderer);
protected:

private:
    // Member data:
    Vector2 m_facingDirection;  // The direction the player is facing
    float m_moveSpeed;
    int m_currentWeapon;
    int m_lives;  // Player's remaining lives
    float m_attackCooldownRemaining;
    float m_attackCooldown;
    std::vector<Projectile*> m_projectiles;  // Store projectiles
    std::vector<MeleeHitbox*> m_meleeHitboxes;  // Store melee hitboxes
public:

protected:

private:

};

#endif // _PLAYER_H_
