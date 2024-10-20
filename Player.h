#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Entity.h"
#include "InputSystem.h"
#include "Vector2.h"
#include "Projectile.h"
#include <vector>  // To store multiple projectiles
#include "sprite.h"
#include "MeleeHitbox.h"
#include "fmod.hpp"
#include "DungeonRoom.h"

class Sprite;

class Player : public Entity
{
public:
    Player();
    ~Player();

// Member methods:
    bool Initialise(Renderer& renderer);
    void Process(float deltaTime, InputSystem& inputSystem, Renderer& renderer, DungeonRoom& dungeonRoom);
    void Attack(Renderer& renderer);
    void Draw(Renderer& renderer);
    int GetLives() const;
    int GetWeapons() const;
    void SetDead();
    void TakeDamage(int damage);
    MeleeHitbox* GetMelee();
    Projectile* GetProjectile();

    std::vector<MeleeHitbox*>& GetMeleeHitboxes();
    std::vector<Projectile*>& GetProjectiles();

protected:

private:

// Member data:
    Vector2 m_facingDirection;  // The direction the player is facing
    float m_invincibilityRemaining;
    float m_moveSpeed;
    int m_currentWeapon;
    int m_lives;  // Player's remaining lives
    float m_attackCooldownRemaining;
    float m_attackCooldown;

    FMOD::System* m_pFmodSystem;
    FMOD::Sound* m_pShootSound;
    FMOD::Sound* m_pMeleeSound;

public:
    std::vector<Projectile*> m_projectiles;  // Store projectiles
    std::vector<MeleeHitbox*> m_melee;  // Store melee hitboxes

    bool m_bAlive;

protected:

private:

};

#endif // _PLAYER_H_