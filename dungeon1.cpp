// This include:
#include "dungeon1.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "Player.h"
#include "Magic.h"

#include "imgui/imgui.h"
#include "renderer.h"

// Library includes:
#include <cassert>
#include <SDL_ttf.h>
#include <SDL.h>
#include <cmath>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

const int TILE_SIZE = 64;

Dungeon1Scene::Dungeon1Scene() :
	m_pCentre(nullptr),
	m_angle(0.0f),
	m_rotationSpeed(0.0f),
	m_Enemy1(nullptr),
	m_Enemy2(nullptr),
	m_pPlayer(nullptr),
	m_pRenderer(nullptr),
	m_pPlayerHPSprite(nullptr),
	m_pPlayerWeaponSprite(nullptr),
	fmodSystem(nullptr),
	musicChannel(nullptr),
	backgroundMusic(nullptr),
	m_bMusicPlaying(false)
{
	// FMOD initialization
	FMOD::System_Create(&fmodSystem);
	fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);


	// Load background music
	fmodSystem->createSound("sounds\\game theme.mp3", FMOD_LOOP_NORMAL, 0, &backgroundMusic);
}

Dungeon1Scene::~Dungeon1Scene()
{
	// Clean up FMOD sounds
	if (backgroundMusic)
	{
		backgroundMusic->release();
	}

	if (fmodSystem)
	{
		fmodSystem->close();
		fmodSystem->release();
	}

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
	for (auto& pAnimatedSprite : m_explosions)
	{
		delete pAnimatedSprite;
	}
	m_explosions.clear();
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
	m_pCentre = renderer.CreateSprite("Sprites\\scenes\\Background.png");


	const int BOARD_HALF_WIDTH = m_pCentre->GetWidth() / 2;
	const int BOARD_HALF_HEIGHT = m_pCentre->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pCentre->SetX(SCREEN_WIDTH / 2);
	m_pCentre->SetY(SCREEN_HEIGHT / 2);

	// Initialize the dungeon room
	m_dungeonRoom.LoadTiles("Rooms\\Room1.json"); // Use LoadTiles instead of CreateFromJSON
	m_ladder.Initialise(renderer); // Initialise the ladder
	m_ladder.SetDead();

	// Initialize the player:
	m_pPlayer = new Player();
	if (!m_pPlayer->Initialise(renderer))
	{
		return false;
	}
	m_pPlayer->GetPosition().x = SCREEN_WIDTH / 2;
	m_pPlayer->GetPosition().y = SCREEN_HEIGHT / 2;

	// Spawn a setting number of enemies:
	SpawnEnemies();

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

bool Dungeon1Scene::AreAllEnemiesDead()
{
	for (auto& m_Enemy1 : m_Enemies1)
	{
		if (m_Enemy1->IsAlive())
		{
			return false;
		}
	}

	for (auto& m_Enemy2 : m_Enemies2)
	{
		if (m_Enemy2->IsAlive())
		{
			return false;
		}
	}

	return true;
}

void Dungeon1Scene::Process(float deltaTime, InputSystem& inputSystem)
{
	// Pass the dungeon room to the player's process for collision checks
	m_pPlayer->Process(deltaTime, inputSystem, *m_pRenderer, m_dungeonRoom);

	for (auto& m_Enemy1 : m_Enemies1)
	{
		m_Enemy1->Process(deltaTime, m_dungeonRoom);  // Pass dungeon room to enemies for collision
	}
	for (auto& m_Enemy2 : m_Enemies2)
	{
		m_Enemy2->Process(deltaTime, m_dungeonRoom);  // Pass dungeon room to enemies for collision
	}

	if (AreAllEnemiesDead())
	{
		SpawnLadder();
	}

	if (m_ladder.IsAlive() && m_pPlayer->IsCollidingWith(m_ladder))
	{
		NewRoom();
	}

	UpdatePlayerWeaponUI();
	CheckCollisions();
	UpdateExplosions(deltaTime);
}


void Dungeon1Scene::Draw(Renderer& renderer)
{
	//draw the background
	m_pCentre->Draw(renderer);

	m_dungeonRoom.Draw(renderer);
	m_ladder.Draw(renderer);

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

	if (m_pPlayerWeaponSprite)
	{
		m_pPlayerWeaponSprite->Draw(renderer); // Draw current weapon
	}

	// Draw each heart sprite in m_pPlayerHP
	for (Sprite* heartSprite : m_pPlayerHP)
	{
		heartSprite->Draw(renderer);
	}

	//draw explosions when enemies dead
	for (auto& explosion : m_explosions)
	{
		explosion->Draw(renderer);
	}
}

void Dungeon1Scene::NewRoom()
{
	// List of room JSON files
	const char* roomFiles[4] = {
		"Rooms\\Room1.json",
		"Rooms\\Room2.json",
		"Rooms\\Room3.json",
		"Rooms\\Room4.json"
	};

	// Seed the random number generator if you haven't done this globally
	srand(static_cast<unsigned int>(time(0)));

	// Select a random index between 0 and 3
	int randomIndex = rand() % 4;

	m_ladder.SetDead();

	// Load a random room
	m_dungeonRoom.LoadTiles(roomFiles[randomIndex]);

	// Call the new method to spawn enemies
	SpawnEnemies();
}


void Dungeon1Scene::SpawnLadder()   // Spawns the ladder in the room
{
	m_ladder.SetPosition(Vector2(740.0f, 640.0f), 0.0f);
	m_ladder.m_pSprite->SetX(740);
	m_ladder.m_pSprite->SetY(640);
	m_ladder.SetAlive();
}

void Dungeon1Scene::SpawnEnemies()
{
	// Clear existing enemies
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

	// Determine number of rows and columns in the tilemap (in tile units)
	int rows = m_dungeonRoom.GetRowCount();  // Assuming a method that returns the number of rows
	int cols = m_dungeonRoom.GetColCount();  // Assuming a method that returns the number of columns

	// Respawn enemies at valid positions
	for (int i = 0; i < 5; i++)
	{
		Vector2 position;
		int tileX, tileY;

		// Randomize until a passable tile is found
		do {
			tileX = rand() % cols;  // Random X in grid units (tiles)
			tileY = rand() % rows;  // Random Y in grid units (tiles)
		} while (!m_dungeonRoom.IsTilePassable(tileX, tileY));

		// Convert tile coordinates to pixel coordinates
		position.x = tileX * TILE_SIZE + TILE_SIZE / 2;  // Center of the tile
		position.y = tileY * TILE_SIZE + TILE_SIZE / 2;  // Center of the tile

		m_Enemy1 = new EnemyS(m_pPlayer);
		m_Enemy1->Initialise(*m_pRenderer);
		m_Enemy1->SetPosition(position, 0.0f);  // Set the valid position
		m_Enemies1.push_back(m_Enemy1);
	}

	for (int i = 0; i < 5; i++)
	{
		Vector2 position;
		int tileX, tileY;

		// Randomize until a passable tile is found
		do {
			tileX = rand() % cols;  // Random X in grid units (tiles)
			tileY = rand() % rows;  // Random Y in grid units (tiles)
		} while (!m_dungeonRoom.IsTilePassable(tileX, tileY));

		// Convert tile coordinates to pixel coordinates
		position.x = tileX * TILE_SIZE + TILE_SIZE / 2;  // Center of the tile
		position.y = tileY * TILE_SIZE + TILE_SIZE / 2;  // Center of the tile

		m_Enemy2 = new EnemySlime(m_pPlayer);
		m_Enemy2->Initialise2(*m_pRenderer);
		m_Enemy2->SetPosition(position, 0.0f);  // Set the valid position
		m_Enemies2.push_back(m_Enemy2);
	}
}



void Dungeon1Scene::DebugDraw()
{
	ImGui::Text("Scene: TitleScene");
}

void Dungeon1Scene::CheckCollisions()
{
	if (m_pPlayer && m_pPlayer->IsAlive())
	{
		// player getting damage:
		for (auto& skeleton : m_Enemies1)
		{
			if (skeleton->IsAlive() && m_pPlayer->IsCollidingWith(*skeleton))
			{
				m_pPlayer->TakeDamage(1);
				UpdatePlayerHPUI();
				if (m_pPlayer->GetLives() == 0)
				{
					m_pPlayer->SetDead();
					(*m_sceneIndex)--;
				}
			}

			Magic* magic = skeleton->GetMagic();
			if (magic)
			{
				if (m_pPlayer->IsCollidingWith(*magic))
				{
					m_pPlayer->TakeDamage(1);
					UpdatePlayerHPUI();
					if (m_pPlayer->GetLives() == 0)
					{
						m_pPlayer->SetDead();
						(*m_sceneIndex)--;
					}
				}
			}
		}

		for (auto& enemySlime : m_Enemies2)
		{
			if (enemySlime->IsAlive() && m_pPlayer->IsCollidingWith(*enemySlime))
			{
				m_pPlayer->TakeDamage(1);
				if (m_pPlayer->GetLives() == 0)
				{
					m_pPlayer->SetDead();
					(*m_sceneIndex)--;
				}
			}
		}

		// enemy getting damage:
		for (auto meleeHitbox : m_pPlayer->GetMeleeHitboxes())
		{
			if (meleeHitbox)
			{
				for (auto enemy : m_Enemies1)
				{
					if (enemy && enemy->IsAlive() && enemy->IsCollidingWith(*meleeHitbox))
					{
						enemy->SetDead();
						CreateExplosion(enemy->GetPosition().x, enemy->GetPosition().y);
					}
				}
				for (auto enemy : m_Enemies2)
				{
					if (enemy && enemy->IsAlive() && enemy->IsCollidingWith(*meleeHitbox))
					{
						enemy->SetDead();
						CreateExplosion(enemy->GetPosition().x, enemy->GetPosition().y);
					}
				}
			}
		}

		for (auto projectile : m_pPlayer->GetProjectiles())
		{
			if (projectile)
			{
				for (auto enemy : m_Enemies1)
				{
					if (enemy && enemy->IsAlive() && enemy->IsCollidingWith(*projectile))
					{
						enemy->SetDead();
						CreateExplosion(enemy->GetPosition().x, enemy->GetPosition().y);
					}
				}
				for (auto enemy : m_Enemies2)
				{
					if (enemy && enemy->IsAlive() && enemy->IsCollidingWith(*projectile))
					{
						enemy->SetDead();
						CreateExplosion(enemy->GetPosition().x, enemy->GetPosition().y);
					}
				}
			}
		}
	}
}

void Dungeon1Scene::OnSceneChange(int* sceneIndex)
{
	m_sceneIndex = sceneIndex;
}

void Dungeon1Scene::PlayBackgroundMusic()
{
	if (!m_bMusicPlaying)
	{
		fmodSystem->playSound(backgroundMusic, 0, false, &musicChannel);
		m_bMusicPlaying = true;
	}
}

void Dungeon1Scene::StopBackgroundMusic()
{
	if (m_bMusicPlaying)
	{
		musicChannel->stop();
		m_bMusicPlaying = false;
	}
}

bool Dungeon1Scene::IsMusicPlaying() const
{
	return m_bMusicPlaying;
}

void Dungeon1Scene::CreateExplosion(float x, float y)
{
	AnimatedSprite* newExplosion = m_pRenderer->CreateAnimatedSprite("Sprites\\spritesheet.png");
	newExplosion->SetupFrames(300, 300);
	newExplosion->SetFrameDuration(0.1f);
	newExplosion->SetLooping(false);
	newExplosion->SetScale(0.4);
	newExplosion->SetX(static_cast<int>(x));
	newExplosion->SetY(static_cast<int>(y));
	newExplosion->Animate();
	m_explosions.push_back(newExplosion);
}

void Dungeon1Scene::UpdateExplosions(float deltaTime)
{
	for (auto it = m_explosions.begin(); it != m_explosions.end();)
	{
		AnimatedSprite* explosion = *it;

		explosion->Process(deltaTime);

		if (!explosion->IsAnimating())
		{
			delete explosion;
			it = m_explosions.erase(it);
		}
		else
		{
			++it;
		}
	}
}