﻿// This include: 
#include "game.h"
#include "sprite.h"
#include "scene.h"
#include "ball.h"
#include "inputsystem.h"
#include "animatedsprite.h"
#include "soundsystem.h"

//Scenes:
#include "TitleScene.h"
#include "dungeon1.h"
#include "InstructionScene.h"
#include "Lose.h"

// Library includes: 
#include "renderer.h" 
#include "logmanager.h"
#include "imgui/imgui_impl_sdl2.h"
#include "fmod.hpp"
#include "fmod.h"
#include "fmod_errors.h"

#include <vector>
#include <SDL_ttf.h>

//namespace using:
using FMOD::System;
using FMOD::Sound;
using FMOD::Channel;
using FMOD::ChannelGroup;

// Static Members:
Game* Game::sm_pInstance = 0;
SoundSystem* Game::pSoundsystem = nullptr;

void SceneChange(int& currentScene)
{
	currentScene++;
}

Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}
	return (*sm_pInstance);
}

void Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game() :
	m_pRenderer(0),
	m_bLooping(true),
	m_pInputSystem(0)
{
	pSoundsystem = new SoundSystem();
}

Game::~Game()
{
	delete m_pRenderer;
	m_pRenderer = 0;

	delete m_pInputSystem;
	m_pInputSystem = 0;

	delete pSoundsystem;
	pSoundsystem = 0;

	delete m_pZapPow[0];
	m_pZapPow[0] = 0;
}

void Game::Quit()
{
	m_bLooping = false;
}

bool Game::Initialise()
{
	//load sound system:
	pSoundsystem->init();

	//define the size of the screen
	int bbWidth = 1860;
	int bbHeight = 1050;

	//Instantiate a input system pointer with new
	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialise();

	m_pRenderer = new Renderer();
	if (!m_pRenderer->Initialise(true, bbWidth, bbHeight))
	{
		LogManager::GetInstance().Log("Renderer failed to initialise!");
		return false;
	}

	//creating the scenes:
	Scene* pScene = 0;
	pScene = new TitleScene();
	pScene->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene);
	m_iCurrentScene = 0;

	Scene* pScene1 = 0; //empty scene to occupy a position
	pScene1 = new TitleScene();
	pScene1->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene);

	Scene* pScene2 = 0;
	pScene2 = new InstructionScene();
	pScene2->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene2);

	Scene* pScene3 = 0;
	pScene3 = new LoseScene();
	pScene3->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene3);

	Dungeon1Scene* pScene4 = 0;
	pScene4 = new Dungeon1Scene();
	pScene4->OnSceneChange(&m_iCurrentScene);
	pScene4->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene4);

	// text renderer at last:
	// Load static text textures into the Texture Manager... 
	m_pRenderer->CreateStaticText("Right Click to Continue", 60);

	// Generate sprites that use the static text textures... 
	m_pZapPow[0] = m_pRenderer->CreateSprite("Right Click to Continue");
	m_pZapPow[0]->SetX(900);
	m_pZapPow[0]->SetY(100);
	m_pZapPow[0]->SetAngle(0);

	bbWidth = m_pRenderer->GetWidth();
	bbHeight = m_pRenderer->GetHeight();

	m_iLastTime = SDL_GetPerformanceCounter();
	m_pRenderer->SetClearColour(0, 255, 255);

	return true;
}

bool Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;
	// TODO: Process input here! 

	m_pInputSystem->ProcessInput();

	if (m_bLooping)
	{
		Uint64 current = SDL_GetPerformanceCounter();
		float deltaTime = (current - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
		m_iLastTime = current;
		m_fExecutionTime += deltaTime;
		Process(deltaTime);

		pSoundsystem->update();

#ifdef USE_LAG
		m_fLag += deltaTime;
		int innerLag = 0;
		while (m_fLag >= stepSize)
		{
			Process(stepSize);

			m_fLag -= stepSize;

			++m_iUpdateCount;
			++innerLag;
		}
#endif //USE_LAG

		Draw(*m_pRenderer);
	}
	return m_bLooping;
}

void Game::PlayMusicGameState()
{
	// Check if the current scene is KugelHolleSceneGame to play music
	Dungeon1Scene* Dungeon1 = dynamic_cast<Dungeon1Scene*>(m_scenes[m_iCurrentScene]);
	if (Dungeon1)
	{
		// Ensure the music is playing only in the game scene
		if (!Dungeon1->IsMusicPlaying())
		{
			Dungeon1->PlayBackgroundMusic();
		}
	}
	else
	{
		// Stop the music if we switch to a different scene
		for (Scene* scene : m_scenes)
		{
			Dungeon1Scene* otherscene = dynamic_cast<Dungeon1Scene*>(scene);
			if (otherscene && otherscene->IsMusicPlaying())
			{
				otherscene->StopBackgroundMusic();
			}
		}
	}
}


void Game::Process(float deltaTime)
{
	ProcessFrameCounting(deltaTime);
	// TODO: Add game objects to process here!
	m_scenes[m_iCurrentScene]->Process(deltaTime, *m_pInputSystem);
	
	PlayMusicGameState();

	//right click mouse to move to next scene:
	if (m_pInputSystem->GetMouseButtonState(SDL_BUTTON_RIGHT) == BS_PRESSED && m_iCurrentScene < m_scenes.size() - 1)
	{
		m_iCurrentScene+=2;
	}

	ButtonState leftArrowState = (m_pInputSystem->GetKeyState(SDL_SCANCODE_LEFT));

	if (leftArrowState == BS_PRESSED)
	{
		LogManager::GetInstance().Log("Left arrow key pressed.");
	}
	else if (leftArrowState == BS_RELEASED)
	{
		LogManager::GetInstance().Log("Left arrow key released.");
	}

	int result = m_pInputSystem->GetMouseButtonState(SDL_BUTTON_LEFT);

	if (result == BS_PRESSED)
	{
		LogManager::GetInstance().Log("Left mouse button pressed.");
	}
	else if (result == BS_RELEASED)
	{
		LogManager::GetInstance().Log("Left mouse button released.");
	}
}

void Game::DebugDraw()
{
	// assigned to be true by myself because it hasn't been return by functions
	if (m_bShowDebugWindow = true)
	{
		bool open = true;
		ImGui::Begin("Debug Window", &open, ImGuiWindowFlags_MenuBar);
		ImGui::Text("My Game Frame", "2024, S2");

		if (ImGui::Button("Quit"))
		{
			Quit();
		}

		ImGui::SliderInt("Active scene", &m_iCurrentScene, 0, m_scenes.size() - 1, "%d");
		m_scenes[m_iCurrentScene]->DebugDraw();

		ImGui::End();
	}
}

void Game::Draw(Renderer& renderer)
{
	++m_iFrameCount;
	renderer.Clear();

	// TODO: Add game objects to draw here!
	m_scenes[m_iCurrentScene]->Draw(renderer);

	//draw the fonts:
	m_pZapPow[0]->Draw(renderer);


	DebugDraw();

	renderer.Present();
}

void Game::ProcessFrameCounting(float deltaTime)
{
	// Count total simulation time elapsed:
	m_fElapsedSeconds += deltaTime;
	// Frame Counter:
	if (m_fElapsedSeconds > 1.0f)
	{
		m_fElapsedSeconds -= 1.0f;
		m_iFPS = m_iFrameCount;
		m_iFrameCount = 0;
	}
}

void Game::ToggleDebugWindow()
{
	m_bShowDebugWindow = !m_bShowDebugWindow;

	m_pInputSystem->ShowMouseCursor(m_bShowDebugWindow);
}
