#ifndef _ENEMYS_H_
#define _ENEMYS_H_


#include "Entity.h"

class EnemyS : public Entity
{
public:
	EnemyS();
	~EnemyS();

	//member methods:
public:
	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);

protected:
	void Attack();
	bool IsWithinRange(Vector2 m_position, Vector2 Player_position);

private:
	bool IsNearBoundary(Vector2 m_position);

	//member data:
public:

protected:
	

private:
	Vector2 m_targetPosition;
	float m_speed;
	float m_moveTimer;
	float m_moveInterval;
	float m_moveDistance;
	float m_moveRange;
	float attack_range;
};

#endif // _ENEMYS_H_