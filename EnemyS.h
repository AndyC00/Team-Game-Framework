#ifndef _ENEMYS_H_
#define _ENEMYS_H_

#include "Magic.h"
#include "Entity.h"
#include "Player.h"


class EnemyS : public Entity
{
public:
	EnemyS(Player* player);
	~EnemyS();

	//member methods:
public:
	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

protected:
	void Shoot(float deltaTime);
	bool IsWithinRange(Vector2 m_position);

private:
	bool IsNearBoundary(Vector2 m_position);

	//member data:
public:

protected:
	

private:
	Vector2 m_targetPosition;
	Vector2 enemyPosition;
	float m_speed;
	float m_moveTimer;
	float m_moveInterval;
	float m_moveDistance;
	float m_moveRange;
	float attack_range;
	float m_MagicTimer;

	Magic* m_pMagic;
	Player* m_pPlayer;
	Renderer* m_pRenderer;
};

#endif // _ENEMYS_H_