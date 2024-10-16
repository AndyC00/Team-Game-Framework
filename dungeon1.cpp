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

	// Initialize hearts based on player's lives
	for (int i = 0; i < m_pPlayer->GetLives(); ++i) {
		m_pPlayerHPSprite = m_pRenderer->CreateSprite("Sprites\\heart.png");
		m_pPlayerHPSprite->SetScale(0.3f);
		m_pPlayerHPSprite->SetX(200 + i * 150);  // Positioning hearts horizontally
		m_pPlayerHPSprite->SetY(50);
		m_pPlayerHP.push_back(m_pPlayerHPSprite);
	}

	// Load static text textures into the Texture Manager... 
	m_pRenderer->CreateStaticText("HP:", 36);

	// Generate sprites that use the static text textures... 
	m_pZapPow[0] = m_pRenderer->CreateSprite("HP:");
	m_pZapPow[0]->SetX(100);
	m_pZapPow[0]->SetY(50);
	m_pZapPow[0]->SetAngle(0);

	// Create static text for weapon label
	m_pRenderer->CreateStaticText("Weapon:", 36);
	m_pZapPow[1] = m_pRenderer->CreateSprite("Weapon:");
	m_pZapPow[1]->SetX(100);
	m_pZapPow[1]->SetY(100);

	return true;
}

void Dungeon1Scene::UpdatePlayerHPUI()
{
	int currentLives = m_pPlayer->GetLives();

	// Remove excess heart sprites when player loses lives
	while (m_pPlayerHP.size() > currentLives) {
		delete m_pPlayerHP.back();
		m_pPlayerHP.pop_back();
	}
}

void Dungeon1Scene::UpdatePlayerWeaponUI()
{
	if (m_pPlayer->GetWeapons() == 1)
	{
		m_pPlayerWeaponSprite = m_pRenderer->CreateSprite("Sprites\\sword.png");
		m_pPlayerWeaponSprite->SetScale(0.2f);
		m_pPlayerWeaponSprite->SetX(250);
		m_pPlayerWeaponSprite->SetY(100);
	}
	else
	{
		m_pPlayerWeaponSprite = m_pRenderer->CreateSprite("Sprites\\gun.png");
		m_pPlayerWeaponSprite->SetScale(0.2f);
		m_pPlayerWeaponSprite->SetX(250);
		m_pPlayerWeaponSprite->SetY(100);
	}
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
	CheckCollisions();
	UpdatePlayerWeaponUI();
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
	//m_pPlayerHPSprite->Draw(renderer); // Draw dynamic player HP

	m_pZapPow[1]->Draw(renderer); // Draw Weapon label
	m_pPlayerWeaponSprite->Draw(renderer); // Draw current weapon

	// Draw each heart sprite in m_pPlayerHP
	for (Sprite* heartSprite : m_pPlayerHP)
	{
		heartSprite->Draw(renderer);
	}
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

	if (m_pPlayer)
	{
		ImGui::Text("Player HP: %d", m_pPlayer->GetLives());
		// Check if the player has taken damage
		if (m_pPlayer->GetLives() < 5) 
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Player took damage!");
		}
		else
		{
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Player is at full health.");
		}
	}
}

void Dungeon1Scene::CheckCollisions()
{
	if (m_pPlayer && m_pPlayer->IsAlive())
	{
		for (auto& enemy : m_Enemies1)
		{
			if (enemy->IsAlive() && m_pPlayer->IsCollidingWith(*enemy))
			{
				m_pPlayer->TakeDamage(1);
				UpdatePlayerHPUI();
				if (m_pPlayer->GetLives() == 0)
				{
					m_pPlayer->SetDead();
				}
			}
		}

		//put a breakpoint here because I see the enemyS in m_Enemies2 has null values
		/*for (auto& enemyS : m_Enemies2)
		{
			if (enemyS->IsAlive() && m_pPlayer->IsCollidingWith(*enemyS))
			{
				m_pPlayer->TakeDamage(1);
				if (m_pPlayer->GetLives() == 0)
				{
					m_pPlayer->SetDead();
				}
			}
		}*/

		/*//enemy collision check:
		for (auto& enemy : m_Enemies1)
		{
			if (enemy->IsAlive() && enemy->IsCollidingWith())
			{
				enemy->SetDead();
			}
		}
		for (auto& enemy : m_Enemies1)
		{
			if (enemy->IsAlive() && enemy->IsCollidingWith())
			{
				enemy->SetDead();
			}
		}*/
	}
}
