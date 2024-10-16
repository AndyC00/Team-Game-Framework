// This include:
#include "dungeon1.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "Player.h"

#include "imgui/imgui.h"
#include "renderer.h"

// Library includes:
#include <cassert>
#include <SDL_ttf.h>
#include<string>

Dungeon1Scene::Dungeon1Scene() :
	m_pCentre(0),
	m_angle(0.0f),
	m_rotationSpeed(0.0f),
	m_Enemy1(nullptr),
	m_Enemy2(nullptr),
	m_pPlayer(nullptr),
	m_pRenderer(nullptr),
	m_pPlayerHPSprite(nullptr),
	m_pPlayerWeaponSprite(nullptr)
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
	for (auto& m_Enemy2 : m_Enemies2)
	{
		delete m_Enemy2;
	}
	m_Enemies2.clear();
	delete m_pPlayer; // Clean up the player

	delete m_pZapPow[0];
	m_pZapPow[0] = 0;

	delete m_pZapPow[1];
	m_pZapPow[1] = 0;

	delete m_pPlayerHPSprite;
	delete m_pPlayerWeaponSprite;
}

bool Dungeon1Scene::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;
	m_pCentre = renderer.CreateSprite("Sprites\\board8x8.png");


	const int BOARD_HALF_WIDTH = m_pCentre->GetWidth() / 2;
	const int BOARD_HALF_HEIGHT = m_pCentre->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pCentre->SetX(SCREEN_WIDTH / 2);
	m_pCentre->SetY(SCREEN_HEIGHT / 2);

	// Initialize the player:
	m_pPlayer = new Player();
	if (!m_pPlayer->Initialise(renderer))
	{
		return false;
	}
	m_pPlayer->GetPosition().x = SCREEN_WIDTH / 2;
	m_pPlayer->GetPosition().y = SCREEN_HEIGHT / 2;

	// Spawn a setting number of enemies:
	for (int i = 0; i < 5; i++)
	{
		m_Enemy1 = new EnemyS(m_pPlayer);
		m_Enemy1->Initialise(renderer);
		m_Enemies1.push_back(m_Enemy1);
	}
	for (int i = 0; i < 5; i++)
	{
		m_Enemy2 = new EnemySlime(m_pPlayer);
		m_Enemy2->Initialise2(renderer);
		m_Enemies2.push_back(m_Enemy2);
	}

	// Load static text textures into the Texture Manager... 
	m_pRenderer->CreateStaticText("HP:", 36);

	// Generate sprites that use the static text textures... 
	m_pZapPow[0] = m_pRenderer->CreateSprite("HP:");
	m_pZapPow[0]->SetX(100);
	m_pZapPow[0]->SetY(50);
	m_pZapPow[0]->SetAngle(0);

	// Create initial HP texture
	m_pPlayerHPSprite = UpdatePlayerHPTexture(m_pPlayer->GetLives());
	m_pPlayerHPSprite->SetX(250);
	m_pPlayerHPSprite->SetY(50);


	// Create static text for weapon label
	m_pRenderer->CreateStaticText("Weapon:", 36);
	m_pZapPow[1] = m_pRenderer->CreateSprite("Weapon:");
	m_pZapPow[1]->SetX(100);
	m_pZapPow[1]->SetY(100);

	// Create initial weapon texture
	m_pPlayerWeaponSprite = UpdatePlayerWeaponTexture(m_pPlayer->GetWeapons());
	m_pPlayerWeaponSprite->SetX(250);
	m_pPlayerWeaponSprite->SetY(100);
	return true;
}

Sprite* Dungeon1Scene::UpdatePlayerHPTexture(int playerHP)
{
	// Convert HP to a string
	std::string hpText = std::to_string(playerHP);

	// Use the renderer to create a new texture for this dynamic text
	m_pRenderer->CreateStaticText(hpText.c_str(), 36);

	// Create a sprite using the newly generated texture
	Sprite* hpSprite = m_pRenderer->CreateSprite(hpText.c_str());

	return hpSprite;
}

Sprite* Dungeon1Scene::UpdatePlayerWeaponTexture(int currentWeapon)
{
	std::string weaponText;
	if (currentWeapon == 1)
	{
		weaponText = "Melee";
	}
	else if (currentWeapon == 2)
	{
		weaponText = "Projectile";
	}

	// Use the renderer to create a new texture for the current weapon
	m_pRenderer->CreateStaticText(weaponText.c_str(), 36);

	// Create a sprite using the newly generated texture
	Sprite* weaponSprite = m_pRenderer->CreateSprite(weaponText.c_str());

	return weaponSprite;
}

void Dungeon1Scene::Process(float deltaTime, InputSystem& inputSystem)
{
	m_pPlayer->Process(deltaTime, inputSystem, *m_pRenderer);

	m_pCentre->SetAngle(0);
	m_pCentre->Process(deltaTime);

	for (auto& m_Enemy1 : m_Enemies1)
	{
		m_Enemy1->Process(deltaTime);
	}
	for (auto& m_Enemy2 : m_Enemies2)
	{
		m_Enemy2->Process(deltaTime);
	}
}

void Dungeon1Scene::Draw(Renderer& renderer)
{
	//draw the background
	m_pCentre->Draw(renderer);

	//draw the enemies
	for (auto& m_Enemy1 : m_Enemies1)
	{
		if (m_Enemy1->IsAlive())
		{
			m_Enemy1->Draw(renderer);
		}
	}
	for (auto& m_Enemy2 : m_Enemies2)
	{
		if (m_Enemy2->IsAlive())
		{
			m_Enemy2->Draw(renderer);
		}
	}

	//draw the player
	m_pPlayer->Draw(renderer);

	m_pZapPow[0]->Draw(renderer); // Draw HP label
	m_pPlayerHPSprite->Draw(renderer); // Draw dynamic player HP

	m_pZapPow[1]->Draw(renderer); // Draw Weapon label
	m_pPlayerWeaponSprite->Draw(renderer); // Draw current weapon
}

void Dungeon1Scene::DebugDraw()
{
	ImGui::Text("Scene: TitleScene");
	ImGui::InputFloat("Rotation speed", &m_rotationSpeed);
	float scale = m_pCentre->GetScale();
	ImGui::SliderFloat("Demo scale", &scale, 0.0f, 2.0f, "%.3f");
	m_pCentre->SetScale(scale);

	int position[2];
	position[0] = m_pCentre->GetX(); position[1] = m_pCentre->GetY();
	ImGui::InputInt2("Demo position", position);
	m_pCentre->SetX(position[0]);
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

//void Dungeon1Scene::CheckCollisions()
//{
//	if (m_pPlayer && m_pPlayer->IsAlive())
//	{
//		for (auto& enemy : m_Enemies1)
//		{
//			if (enemy->IsAlive() && m_pPlayer->IsCollidingWith(*enemy))
//			{
//				//todo: decrease player's hp ... m_pPlayer->GetLives
//				//if(hp<=0)
//				//{
//					m_pPlayer->SetDead();
//				//}
//			}
//		}
//		for (auto& enemy : m_Enemies2)
//		{
//			if (enemy->IsAlive() && m_pPlayer->IsCollidingWith(*enemy))
//			{
//				//todo: decrease player's hp ... m_pPlayer->GetLives
//				//if(hp<=0)
//				//{
//				m_pPlayer->SetDead();
//				//}
//			}
//		}
//
//		//enemy collision check:
//		for (auto& enemy : m_Enemies1)
//		{
//			if (enemy->IsAlive() && enemy->IsCollidingWith())
//			{
//				enemy->SetDead();
//			}
//		}
//		for (auto& enemy : m_Enemies1)
//		{
//			if (enemy->IsAlive() && enemy->IsCollidingWith())
//			{
//				enemy->SetDead();
//			}
//		}
//	}
//}
