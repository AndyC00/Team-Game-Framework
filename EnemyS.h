#ifndef _ENEMYS_H_
#define _ENEMYS_H_

#include "Magic.h"
#include "Entity.h"
#include "Player.h"
#include "fmod.hpp"


class EnemyS : public Entity
{
public:
	EnemyS(Player* player);
	~EnemyS();

	//member methods:
public:
	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, DungeonRoom& dungeonRoom);
	void Draw(Renderer& renderer);
	bool IsAlive() const;
	void SetDead();
	bool IsCollidingWith(Entity& toCheck);
	Magic* GetMagic();

protected:
	void CreateMagic();
	bool IsWithinRange();

private:
	bool IsNearBoundary(Vector2 m_position);

	//member data:
public:
	bool m_bAlive;

protected:
	

private:
	Vector2 m_targetPosition;
	float m_speed;
	float m_moveTimer;
	float m_moveInterval;
	float m_moveDistance;
	float m_moveRange;
	float m_MagicTimer;
	float m_attackRange;

	Magic* m_pMagic;
	Player* m_pPlayer;
	Renderer* m_pRenderer;
	FMOD::System* m_pFmodSystem;
	FMOD::Sound* m_pAttackSound;
};

#endif // _ENEMYS_H_