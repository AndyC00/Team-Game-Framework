//local includes:
#include "Magic.h"
#include "sprite.h"
#include "animatedsprite.h"

//library includes:
#include "renderer.h" 
#include <cmath>

Magic::Magic()
	: m_isAlive(true)
	, m_lifetime(2.0f)  // lifetime setting
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

	newMagic = m_pRenderer->CreateAnimatedSprite("Sprites\\fireExplosion.png");
	newMagic->SetupFrames(128, 128);
	newMagic->SetFrameDuration(0.1f);
	newMagic->SetLooping(false);
	newMagic->Animate();

	return (newMagic != nullptr);
}

void Magic::SetPosition(const Vector2& position, float angle)
{
	m_position = position;
	m_angle = angle;

	const float SPEED = 800;
	float angleInRadians = -m_angle * 3.14159f / 180.0f;
	m_velocity.x = sin(angleInRadians) * SPEED;
	m_velocity.y = cos(angleInRadians) * SPEED;
}

void Magic::Process(float deltaTime)
{

	m_position += m_velocity * deltaTime;

	newMagic->SetX(m_position.x);
	newMagic->SetY(m_position.y);

	UpdateExplosions(deltaTime);
}

void Magic::Draw(Renderer& renderer)
{
	newMagic->Draw(renderer);
}

bool Magic::IsAlive() const
{
	return m_isAlive;
}

Vector2& Magic::GetPosition()
{
	return m_position;
}

float Magic::GetRadius()
{
	return newMagic->GetWidth() / 2;
}

void Magic::UpdateExplosions(float deltaTime)
{

	newMagic->Process(deltaTime);

	if (!newMagic->IsAnimating())
	{
		newMagic->Restart();
	}
}