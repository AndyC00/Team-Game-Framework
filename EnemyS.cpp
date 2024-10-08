// This include:
#include "EnemyS.h"
#include "sprite.h"

// Local includes:
#include "entity.h"
#include "vector2.h"
#include "Magic.h"
#include "Player.h"

// Library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <ctime> 

//how often for the enemy to shoot
const float TimerTotal = 3.0f;
Player m_pPlayer;

EnemyS::EnemyS() :Entity(),
m_moveTimer(0.0f),
m_moveInterval(2.0f),
m_speed(35.0f),
m_moveDistance(50.0f),
m_moveRange(80.0f),
attack_range(30.0f),
m_pMagic(nullptr),
m_MagicTimer(0)
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
	enemyPosition = Vector2(m_x, m_y);
	m_targetPosition = enemyPosition;
	m_velocity = Vector2(0.0f, 0.0f);

	m_pMagic = new Magic();
	m_pMagic->Initialise(renderer);

	return true;
}

void EnemyS::Process(float deltaTime)
{
	if (IsAlive())
	{
		m_moveTimer += deltaTime;

		//calculate if the player come within the enemy's attack range:
		if (IsWithinRange(enemyPosition))
		{
			Shoot(deltaTime);
			//heading to the player:

		}

		if (m_moveTimer >= m_moveInterval)
		{
			m_moveTimer = 0.0f;

			float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * static_cast<float>(M_PI);

			Vector2 displacement = Vector2(cos(angle), sin(angle)) * m_moveDistance;

			Vector2 potentialPosition = enemyPosition + displacement;

			if ((potentialPosition - enemyPosition).Length() <= m_moveRange)
			{
				m_targetPosition = potentialPosition;
			}
			else
			{
				displacement.Normalise();
				displacement *= m_moveRange;
				m_targetPosition = enemyPosition + displacement;
			}
		}

		Vector2 direction = m_targetPosition - enemyPosition;
		float distanceToTarget = direction.Length();

		if (distanceToTarget > 0.1f)
		{
			direction.Normalise();

			Vector2 movement = direction * m_speed * deltaTime;

			if (movement.Length() > distanceToTarget)
			{
				enemyPosition = m_targetPosition;
			}
			else
			{
				enemyPosition += movement;
			}
		}

		m_pSprite->SetX(static_cast<int>(enemyPosition.x));
		m_pSprite->SetY(static_cast<int>(enemyPosition.y));

	}
}

void EnemyS::Draw(Renderer& renderer)
{
	m_pMagic->Draw(renderer);
	Entity::Draw(renderer);
}

bool EnemyS::IsNearBoundary(Vector2 enemyPosition)
{
	float margin = 35.0f; //the distance to trigger the function

	return (enemyPosition.x <= margin || enemyPosition.x >= 1860.0f - margin ||
		enemyPosition.y <= margin || enemyPosition.y >= 1060.0f - margin);
}

bool EnemyS::IsWithinRange(Vector2 enemyPosition)
{
	Vector2 PlayerPosition = m_pPlayer.GetPosition();
	float attackRange = 20.0f;			// the range of attack detection

	float deltaX = enemyPosition.x - PlayerPosition.x;
	float deltaY = enemyPosition.y - PlayerPosition.y;
	float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

	if (distance <= attackRange)
	{
		return true;
	}

	return false;
}

void EnemyS::Shoot(float deltaTime)
{
	if (m_MagicTimer < 0)
	{
		m_pMagic->SetPosition(enemyPosition, m_pSprite->GetAngle());
		m_MagicTimer = TimerTotal;
	}
	else
	{
		m_pMagic->Process(deltaTime);
		m_MagicTimer -= deltaTime;
	}
}