#ifndef _ENEMYSLIME_H_
#define _ENEMYSLIME_H_

#include "Entity.h"
#include "Player.h"


class EnemySlime : public Entity
{
public:
	EnemySlime(Player* player);
	~EnemySlime();

	//member methods:
public:
	bool Initialise2(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	bool IsNearBoundary(Vector2 m_position);
	bool IsAlive() const;
	void SetDead();
	bool IsCollidingWith(Entity& toCheck);

protected:
	bool IsWithinRange();

private:


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
	float m_attackRange;

	Player* m_pPlayer;
	AnimatedSprite* m_slime;
	Renderer* m_pRenderer;
};

#endif // _ENEMYSLIME_H_