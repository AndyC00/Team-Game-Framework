#include "dungeon1scene.h"
#include "renderer.h"
#include "imgui/imgui.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "inputsystem.h"

Dungeon1Scene::Dungeon1Scene() : m_dungeonRoom()
{
}

Dungeon1Scene::~Dungeon1Scene()
{
}

bool Dungeon1Scene::Initialise(Renderer& renderer)
{
    // Initialize the dungeon room
    m_dungeonRoom.LoadTiles("Rooms\\Room1.json"); // Use LoadTiles instead of CreateFromJSON
    m_ladder.Initialise(renderer); // Initialise the ladder
    m_ladder.SetDead();

    return true;
}

void Dungeon1Scene::Process(float deltaTime, InputSystem& inputSystem)
{
    if (inputSystem.GetMouseButtonState(SDL_BUTTON_RIGHT) == BS_PRESSED)    //TEST - THIS IS COLLISION CHECKING WITH THE WALLS
    {

        const Vector2& mousePosition = inputSystem.GetMousePosition();

        // Pass the mouse position to the dungeon room's OnTileClicked method
        m_dungeonRoom.OnTileClicked(static_cast<int>(mousePosition.x), static_cast<int>(mousePosition.y));

    }
    if (inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT) == BS_PRESSED)     //TEST - THIS IS ROOM LOADING AND SHOULD BE REPLACED WITH A COLLISION CHECK WITH THE LADDER
    {
        NewRoom();
    }
    if (inputSystem.GetKeyState(SDL_SCANCODE_SPACE) == BS_PRESSED)		  //TEST - THIS IS LADDER SPAWNING AND SHOULD BE REPLACED WITH CHECK FOR DEAD ENEMIES
    {
        SpawnLadder();
    }

    // Other game logic can go here
}

void Dungeon1Scene::Draw(Renderer& renderer)    // Draw the dungeon room and ladder
{
    m_dungeonRoom.Draw(renderer);
    m_ladder.Draw(renderer);
}

void Dungeon1Scene::DebugDraw()
{
    ImGui::Text("Scene: Dungeon1Scene");
}

void Dungeon1Scene::NewRoom()   // Loads a random room - Need to add enemy spawning and ladder despawning potentially?
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
}

void Dungeon1Scene::SpawnLadder()   // Spawns the ladder in the room
{
    m_ladder.SetPosition(Vector2(320.0f, 320.0f), 0.0f);
    m_ladder.m_pSprite->SetX(320);
    m_ladder.m_pSprite->SetY(320);
    m_ladder.SetAlive();
}