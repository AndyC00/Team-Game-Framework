//local includes:
#include "Magic.h"
#include "sprite.h"
#include "animatedsprite.h"

//library includes:
#include "renderer.h"
#include "vector2.h"
#include "logmanager.h"

#include <cmath>


Magic::Magic()
	: m_angle(0.0f)
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

	newMagic = m_pRenderer->CreateAnimatedSprite("Sprites\\explosion.png");

	if (newMagic)
	{
		newMagic->SetupFrames(128, 128);
		newMagic->SetFrameDuration(0.1f);
		newMagic->SetLooping(false);
		newMagic->Animate();
	}
	else
	{
		LogManager::GetInstance().Log("Magic failed to create!");
	}

	return true;
}

void Magic::SetPosition(const Vector2& position, float angle)
{
	m_position = position;
	m_angle = angle;

	const float SPEED = 300;
	float angleInRadians = -m_angle * M_PI / 180.0f;
	m_velocity.x = sin(angleInRadians) * SPEED;
	m_velocity.y = cos(angleInRadians) * SPEED;
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