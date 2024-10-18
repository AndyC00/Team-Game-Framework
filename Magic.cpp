//local includes:
#include "Magic.h"
#include "sprite.h"
#include "animatedsprite.h"

//library includes:
#include "renderer.h"
#include "vector2.h"
#include "logmanager.h"

#include <cmath>
#include <iostream>


Magic::Magic() :Entity()
	, m_angle(0.0f)
	, m_pRenderer(nullptr)
	, newMagic(nullptr)
{

}

Magic::~Magic()
{
	if (newMagic)
	{
		delete newMagic;
		newMagic = nullptr;
	}
}

bool Magic::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	if (newMagic)
	{
		delete newMagic;
		newMagic = nullptr;
	}

	newMagic = m_pRenderer->CreateAnimatedSprite("Sprites\\fireExplosion.png");

	if (newMagic)
	{
		newMagic->SetupFrames(128, 128);
		newMagic->SetFrameDuration(0.1f);
		newMagic->SetLooping(false);
		newMagic->Animate();

		return true;
	}
	else
	{
		LogManager::GetInstance().Log("Magic failed to create!");
		return false;
	}
}

void Magic::SetPosition(const Vector2& position, float angle)
{
	m_position = position;
	m_angle = angle;

	const float SPEED = 300;
	float angleInRadians = m_angle * M_PI / 180.0f;
	m_velocity.x = cos(angleInRadians) * SPEED;
	m_velocity.y = sin(angleInRadians) * SPEED;

	//std::cout << "Magic SetPosition called:" << std::endl;
	//std::cout << "Position: (" << m_position.x << ", " << m_position.y << ")" << std::endl;
	//std::cout << "Angle: " << m_angle << " degrees (" << angleInRadians << " radians)" << std::endl;
	//std::cout << "Velocity: (" << m_velocity.x << ", " << m_velocity.y << ")" << std::endl;
}

void Magic::Process(float deltaTime)
{
	m_position += m_velocity * deltaTime;

	newMagic->SetX(m_position.x);
	newMagic->SetY(m_position.y);

	newMagic->Process(deltaTime);
}

void Magic::Draw(Renderer& renderer)
{
	//std::cout << "Magic::Draw called at position (" << m_position.x << ", " << m_position.y << ")." << std::endl;
	newMagic->Draw(renderer);
}

const Vector2& Magic::GetPosition() const
{
	return m_position;
}

float Magic::GetRadius()
{
	return newMagic->GetWidth() / 2;
}

bool Magic::IsAnimating() const
{
	if (newMagic)
	{
		return newMagic->IsAnimating();
	}
	return false;
}

float Magic::GetRadius() const
{
	if (newMagic)
	{
		int frameWidth = 128;
		int frameHeight = 128;

		float scale = newMagic->GetScale();

		float actualWidth = frameWidth * scale;
		float actualHeight = frameHeight * scale;

		float radius = (actualWidth + actualHeight) / 4.0f;

		return radius;
	}
	else
	{
		return 0.0f;
	}
}
