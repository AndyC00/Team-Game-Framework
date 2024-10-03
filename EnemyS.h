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

protected:

private:
	void Process(float deltaTime);
	bool IsNearBoundary(Vector2 m_position);

	//member data:
public:

protected:

private:
	float m_moveTimer;
	Vector2 m_targetPosition;
	float m_speed;

};

#endif // _ENEMYS_H_