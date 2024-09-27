#ifndef   ENTITY_H 
#define   ENTITY_H 

// Local includes:
#include "entity.h"
#include "vector2.h"

// Forward declarations:
class Renderer;
class Sprite;

// Class declaration:
class Entity
{
	// Member methods: 
public:
	Entity();
	~Entity();

	bool Initialise(Renderer& renderer, const char* pcFilename);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void Rotate(float direction);
	bool IsAlive() const;
	void SetDead();

	float GetRadius();
	float GetAngle() const;
	void SetAngle(float angle);
	void SetPosition(const Vector2& position, float angle);

	Vector2 GetFacingDirection();

	Vector2& GetPosition();
	Vector2& GetVelocity();

	bool IsCollidingWith(Entity& toCheck);

protected:

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	// Member data: 
public:
	Sprite* m_pSprite;

protected:

	Vector2 m_position;
	Vector2 m_velocity;
	bool m_bAlive;

private:
	float m_angle;
};

#endif //   ENTITY_H 
