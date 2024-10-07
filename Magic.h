#ifndef MAGIC_H
#define MAGIC_H

#include "Vector2.h"
#include "sprite.h"
#include "texture.h"
#include "animatedsprite.h"

class Magic
{
public:
    Magic();
    ~Magic();

    bool Initialise(Renderer& renderer);
    void Process(float deltaTime);
    void Draw(Renderer& renderer);
    bool IsAlive() const;
    void SetPosition(const Vector2& position, float angle);
    Vector2& GetPosition();
    float GetRadius();
    void UpdateExplosions(float deltaTime);

protected:
    Renderer* m_pRenderer;

private:
    Vector2 m_position;
    Vector2 m_velocity;
    bool m_isAlive;
    float m_lifetime;
    float m_angle;
    AnimatedSprite* newMagic;

};

#endif MAGIC_H
