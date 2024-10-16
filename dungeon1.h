#ifndef _DUNGEON1_H_
#define _DUNGEON1_H_

// Local includes:
#include "scene.h"
#include "EnemyS.h"
#include "Player.h"
#include "renderer.h"

// Library includes:
#include <vector>

// Forward declarations:
class Renderer;
class Sprite;
class EnemyS;
class Player;

// Class declaration:
class Dungeon1Scene : public Scene
{
public:
    Dungeon1Scene();
    virtual ~Dungeon1Scene();

    virtual bool Initialise(Renderer& renderer);
    virtual void Process(float deltaTime, InputSystem& inputSystem);
    virtual void Draw(Renderer& renderer);

protected:

private:
    Dungeon1Scene(const Dungeon1Scene& Dungeon1Scene);
    Dungeon1Scene& operator = (const Dungeon1Scene& Dungeon1Scene);
    void CheckCollisions();

    // Member data:
public:
    virtual void DebugDraw();

protected:
    Sprite* m_pCentre;
    float m_angle;
    float m_rotationSpeed;

    std::vector<EnemyS*> m_Enemies1;
    std::vector<EnemySlime*> m_Enemies2;
    Player* m_pPlayer; 
    Renderer* m_pRenderer;

private:
    EnemyS* m_Enemy1;
    EnemySlime* m_Enemy2;
};

#endif // _DUNGEON1_H_
