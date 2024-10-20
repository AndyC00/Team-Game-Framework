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

const int TILE_SIZE = 64;

EnemyS::EnemyS(Player* player) :Entity(),
m_pRenderer(nullptr),
m_pPlayer(player),
m_moveTimer(0.0f),
m_moveInterval(2.0f),
m_speed(35.0f),
m_moveDistance(50.0f),
m_moveRange(80.0f),
m_pMagic(nullptr),
m_MagicTimer(2.0f),
m_attackRange(80.0f),		// the range of attack detection
m_pFmodSystem(nullptr),
m_pAttackSound(nullptr)
{
	m_bAlive = true;
	// Initialize FMOD system
	FMOD::System_Create(&m_pFmodSystem);
	m_pFmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);

	// Load sound
	m_pFmodSystem->createSound("sounds\\explosion.mp3", FMOD_DEFAULT, 0, &m_pAttackSound);
}

EnemyS::~EnemyS()
{
	if (m_pMagic)
	{
		delete m_pMagic;
		m_pMagic = nullptr;
	}

	m_pAttackSound->release();
	m_pFmodSystem->close();
	m_pFmodSystem->release();
}

bool EnemyS::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

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
	m_targetPosition = m_position;
	m_velocity = Vector2(0.0f, 0.0f);


	return true;
}

void EnemyS::Process(float deltaTime, DungeonRoom& dungeonRoom)
{
    if (IsAlive())
    {
        // Enemy attack logic
        m_MagicTimer -= deltaTime;

        if (m_pMagic)
        {
            m_pMagic->Process(deltaTime);
            if (!m_pMagic->IsAnimating())
            {
                delete m_pMagic;
                m_pMagic = nullptr;
                m_MagicTimer = 2.0f; // Reset the timer
            }
        }
        else if (m_MagicTimer <= 0.0f && IsWithinRange())
        {
            CreateMagic();
        }

        // Enemy movement logic
        if (IsNearBoundary(m_position))
        {
            const float screenWidth = 1860.0f;
            const float screenHeight = 1050.0f;
            m_targetPosition = Vector2(screenWidth / 2.0f, screenHeight / 2.0f);
        }
        else
        {
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

        // Calculate movement direction and distance to target
        Vector2 direction = m_targetPosition - m_position;
        float distanceToTarget = direction.Length();

        if (distanceToTarget > 0.1f)
        {
            direction.Normalise();

            Vector2 movement = direction * m_speed * deltaTime;
            Vector2 newPosition = m_position + movement;

            // Convert new position to tile coordinates for collision checking
            int tileX = static_cast<int>(newPosition.x / TILE_SIZE);
            int tileY = static_cast<int>(newPosition.y / TILE_SIZE);

            // Check if the new tile is passable before updating position
            if (dungeonRoom.IsTilePassable(tileX, tileY))
            {
                if (movement.Length() > distanceToTarget)
                {
                    m_position = m_targetPosition;
                }
                else
                {
                    m_position += movement;
                }
            }
        }

        m_pSprite->SetX(static_cast<int>(m_position.x));
        m_pSprite->SetY(static_cast<int>(m_position.y));
    }
    else
    {
        std::cout << "Enemy failed to process!" << std::endl;
    }
}


void EnemyS::Draw(Renderer& renderer)
{
	if (m_pMagic)
	{
		m_pMagic->Draw(renderer);
	}
	if (IsAlive())
	{
		Entity::Draw(renderer);
	}
}

bool EnemyS::IsNearBoundary(Vector2 m_position)
{
	float margin = 100.0f;	//the distance to trigger the function

	return (m_position.x <= margin || m_position.x >= 1860.0f - margin ||
		m_position.y <= margin || m_position.y >= 1050.0f - margin);
}

bool EnemyS::IsWithinRange()
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

void EnemyS::CreateMagic()
{
		if (m_pMagic)
		{
			delete m_pMagic;
			m_pMagic = nullptr;
		}

		m_pMagic = new Magic();
		m_pMagic->Initialise(*m_pRenderer);

		Vector2 direction = m_pPlayer->GetPosition() - m_position;
		direction.Normalise();
		float angle = atan2f(direction.y, direction.x) * (180.0f / static_cast<float>(M_PI));

		m_pMagic->SetPosition(m_position, angle);
		m_pFmodSystem->playSound(m_pAttackSound, 0, false, nullptr);
		m_pFmodSystem->update();

		m_MagicTimer = 2.0f;


		std::cout << "Magic created at position: (" << m_position.x << ", " << m_position.y << ")" << std::endl;
}

bool EnemyS::IsAlive() const
{
	return m_bAlive;
}

void EnemyS::SetDead()
{
	m_bAlive = false;

	if (m_pMagic)
	{
		delete m_pMagic;
		m_pMagic = nullptr;
	}
}

bool EnemyS::IsCollidingWith(Entity& toCheck)
{
	return Entity::IsCollidingWith(toCheck);
}

Magic* EnemyS::GetMagic()
{
	return m_pMagic;
}