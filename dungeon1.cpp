//this includes:
#include "dungeon1.h"

//local includes:
#include "renderer.h"
#include "sprite.h"
#include "EnemyS.h"

#include "imgui/imgui.h"

//library includes:
#include <cassert>

Dungeon1Scene::Dungeon1Scene() :
	m_pCentre(0),
	m_angle(0.0f),
	m_rotationSpeed(0.0f),
	m_Enemy1(nullptr)
{

}

Dungeon1Scene::~Dungeon1Scene()
{
	delete m_pCentre;
	for (auto& m_Enemy1 : m_Enemies1)
	{
		delete m_Enemy1;
	}
	m_Enemies1.clear();
}

bool Dungeon1Scene::Initialise(Renderer& renderer)
{
	m_pCentre = renderer.CreateSprite("Sprites\\board8x8.png");

	const int BOARD_HALF_WIDTH = m_pCentre->GetWidth() / 2;
	const int BOARD_HALF_HEIGHT = m_pCentre->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pCentre->SetX(SCREEN_WIDTH / 2);
	m_pCentre->SetY(SCREEN_HEIGHT / 2);

	// Spawn a setting number of enemies:
	for (int i = 0; i < 10; i++)
	{
		EnemyS* m_Enemy1 = new EnemyS();
		m_Enemy1->Initialise(renderer);
		m_Enemies1.push_back(m_Enemy1);

		m_Enemy1->GetPosition().x = renderer.GetWidth() / 2.0f;
		m_Enemy1->GetPosition().y = renderer.GetHeight() / 2.0f;
	}

	return true;
}
void Dungeon1Scene::Process(float deltaTime, InputSystem& inputSystem)
{
	m_pCentre->SetAngle(0);
	m_pCentre->Process(deltaTime);


	for (auto& m_Enemy1 : m_Enemies1)
	{
		m_Enemy1->Process(deltaTime);
	}
}

void Dungeon1Scene::Draw(Renderer& renderer)
{
	m_pCentre->Draw(renderer);

	for (auto& m_Enemy1 : m_Enemies1)
	{
		if (m_Enemy1->IsAlive())
		{
			m_Enemy1->Draw(renderer);
		}
	}
}

void Dungeon1Scene::DebugDraw()
{
	ImGui::Text("Scene: TitleScene");
	ImGui::InputFloat("Rotation speed", &m_rotationSpeed); float scale = m_pCentre->GetScale();
	ImGui::SliderFloat("Demo scale", &scale, 0.0f, 2.0f, "%.3f"); m_pCentre->SetScale(scale);

	int position[2];
	position[0] = m_pCentre->GetX(); position[1] = m_pCentre->GetY();
	ImGui::InputInt2("Demo position", position); m_pCentre->SetX(position[0]);
	m_pCentre->SetY(position[1]);

	float tint[4];
	tint[0] = m_pCentre->GetRedTint();
	tint[1] = m_pCentre->GetGreenTint();
	tint[2] = m_pCentre->GetBlueTint();
	tint[3] = m_pCentre->GetAlpha();
	ImGui::ColorEdit4("Demo tint", tint);
	m_pCentre->SetRedTint(tint[0]);
	m_pCentre->SetGreenTint(tint[1]);
	m_pCentre->SetBlueTint(tint[2]);
	m_pCentre->SetAlpha(tint[3]);
}
