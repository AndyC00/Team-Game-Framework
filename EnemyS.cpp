// This include:
#include "EnemyS.h"

// Local includes:
#include "entity.h"

// Library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>


EnemyS::EnemyS() :Entity(),
	m_moveTimer(0.0f), 
	m_speed(30.0f)
{

}

EnemyS::~EnemyS()
{

}

bool EnemyS::Initialise(Renderer& renderer)
{
	const char* pcFilename = "Sprites\\skeleton.png";

	if (!Entity::Initialise(renderer, pcFilename))
	{
		std::cout << "Enemy failed to spawn" << std::endl;
		return false;
	}

	//enemy spawning position:
	int m_x = ((rand() % 2 == 0) ? (rand() % 890 + 10) : (rand() % 890 + 910));
	int m_y = ((rand() % 2 == 0) ? (rand() % 490 + 10) : (rand() % 480 + 550));
	m_position = Vector2(m_x, m_y);
	m_velocity = Vector2(0.0f, 0.0f);

	return true;
}

void EnemyS::Process(float deltaTime)
{
	if (IsAlive())
	{
		m_moveTimer += deltaTime;

		//go back if near the edge of the screen
		if (IsNearBoundary(m_position))
		{
			Vector2 center(930.0f, 530.0f); // the center of the screen (1860/2, 1060/2)
			Vector2 direction = center - m_position;
			float angleToCenter = atan2(direction.y, direction.x) * 180.0f / M_PI - 90.0f;
		}
		else
		{
			m_position += m_velocity * deltaTime;
		}

		Entity::Process(deltaTime);
	}
}

bool EnemyS::IsNearBoundary(Vector2 m_position)
{
	float margin = 35.0f; //the distence to trigger the function

	return (m_position.x <= margin || m_position.x >= 1860.0f - margin ||
		m_position.y <= margin || m_position.y >= 1060.0f - margin);
}