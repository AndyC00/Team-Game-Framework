#ifndef _LOSESCENE_H_
#define _LOSESCENE_H_

//local includes:
#include "scene.h"

//Forward declarations:
class Renderer;
class Sprite;

//Class declaration:
class LoseScene :public Scene
{
	//member methods:
public:
	LoseScene();
	virtual ~LoseScene();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

protected:

private:
	LoseScene(const LoseScene& TitleScene);
	LoseScene& operator = (const LoseScene& TitleScene);

	//member data:
public:
	virtual void DebugDraw();

protected:
	Sprite* m_pCentre;

	float m_angle;
	float m_rotationSpeed;

private:

};

#endif // _LOSESCENE_H_
