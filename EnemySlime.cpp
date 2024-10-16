// This include:
#include "EnemySlime.h"
#include "animatedsprite.h"

// Local includes:
#include "entity.h"
#include "vector2.h"
#include "Player.h"

// Library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>


EnemySlime::EnemySlime(Player* player) :Entity(),
m_pRenderer(nullptr),
m_slime(nullptr),
m_pPlayer(player),
m_moveTimer(0.0f),
m_moveInterval(2.0f),
m_speed(35.0f),
m_moveDistance(50.0f),
m_moveRange(150.0f),
m_attackRange(200.0f)		// the range of attack detection
{
	m_bAlive = true;
}

EnemySlime::~EnemySlime()
{
	
}

bool EnemySlime::Initialise2(Renderer& renderer)
{
	m_pRenderer = &renderer;

	m_slime = m_pRenderer->CreateAnimatedSprite("Sprites\\slime.png");
	m_slime->SetupFrames(47, 47);
	m_slime->SetScale(1.65);
	m_slime->SetFrameDuration(0.15f);
	m_slime->SetLooping(true);
	m_slime->Animate();

	//enemy spawning position:
	int m_x = ((rand() % 2 == 0) ? (rand() % 890 + 10) : (rand() % 890 + 910));
	int m_y = ((rand() % 2 == 0) ? (rand() % 490 + 10) : (rand() % 480 + 550));

	m_position = Vector2(m_x, m_y);
	m_targetPosition = m_position;
	m_velocity = Vector2(0.0f, 0.0f);

	m_slime->SetX(static_cast<int>(m_x));
	m_slime->SetY(static_cast<int>(m_y));

	return true;
}

void EnemySlime::Process(float deltaTime)
{
	if (IsAlive())
	{
		m_slime->Process(deltaTime);

		if (IsNearBoundary(m_position))
		{
			const float screenWidth = 1860.0f;
			const float screenHeight = 1050.0f;
			m_targetPosition = Vector2(screenWidth / 2.0f, screenHeight / 2.0f);
		}
		else
		{
			//enemy movement:
			if (IsWithinRange())
			{
				m_targetPosition = m_pPlayer->GetPosition();
			}
			else
			{
				m_moveTimer += deltaTime;

				if (m_moveTimer >= m_moveInterval)
				{
					m_moveTimer = 0.0f;

					float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * static_cast<float>(M_PI);
					Vector2 displacement = Vector2(cos(angle), sin(angle)) * m_moveDistance;
					Vector2 potentialPosition = m_position + displacement;

					if ((potentialPosition - m_position).Length() <= m_moveRange)
					{
						m_targetPosition = potentialPosition;
					}
					else
					{
						displacement.Normalise();
						displacement *= m_moveRange;
						m_targetPosition = m_position + displacement;
					}
				}
			}
		}

		Vector2 direction = m_targetPosition - m_position;
		float distanceToTarget = direction.Length();

		if (distanceToTarget > 0.1f)
		{
			direction.Normalise();

			Vector2 movement = direction * m_speed * deltaTime;

			if (movement.Length() > distanceToTarget)
			{
				m_position = m_targetPosition;
			}
			else
			{
				m_position += movement;
			}
		}

		m_slime->SetX(static_cast<int>(m_position.x));
		m_slime->SetY(static_cast<int>(m_position.y));
		//std::cout << "Enemy Position: (" << m_position.x << ", " << m_position.y << ")" << std::endl;
	}
	else
	{
		std::cout << "Enemy failed to process!" << std::endl;
	}

}

void EnemySlime::Draw(Renderer& renderer)
{
	m_slime->Draw(renderer);
}

bool EnemySlime::IsNearBoundary(Vector2 m_position)
{
	float margin = 100.0f;	//the distance to trigger the function

	return (m_position.x <= margin || m_position.x >= 1860.0f - margin ||
		m_position.y <= margin || m_position.y >= 1050.0f - margin);
}

bool EnemySlime::IsWithinRange()
{
	if (m_pPlayer == nullptr)
	{
		std::cout << "Can't find Player!" << std::endl;
		return false;
	}
	Vector2 PlayerPosition = m_pPlayer->GetPosition();

	float deltaX = m_position.x - PlayerPosition.x;
	float deltaY = m_position.y - PlayerPosition.y;
	float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

	return distance <= m_attackRange;
}

bool EnemySlime::IsAlive() const
{
	return m_bAlive;
}

void EnemySlime::SetDead()
{
	m_bAlive = false;
}

bool EnemySlime::IsCollidingWith(Entity& toCheck)
{
	Entity::IsCollidingWith(toCheck);
}