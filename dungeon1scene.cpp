#include "dungeon1scene.h"
#include "renderer.h"
#include "imgui/imgui.h"
#include <iostream> // Include iostream for std::cerr and std::endl
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

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

    return true;
}

void Dungeon1Scene::Process(float deltaTime, InputSystem& inputSystem)
{
    // Check for mouse click
    if (inputSystem.GetMouseButtonState(SDL_BUTTON_RIGHT) == BS_PRESSED)
    {
        // Get the mouse position from the input system
        const Vector2& mousePosition = inputSystem.GetMousePosition();

        // Pass the mouse position to the dungeon room's OnTileClicked method
        m_dungeonRoom.OnTileClicked(static_cast<int>(mousePosition.x), static_cast<int>(mousePosition.y));

    }
    if (inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT) == BS_PRESSED)
    {
        NewRoom();
    }
    // Other game logic can go here
}

void Dungeon1Scene::Draw(Renderer& renderer)
{
    // Draw the dungeon room
    m_dungeonRoom.Draw(renderer);
}

void Dungeon1Scene::DebugDraw()
{
    ImGui::Text("Scene: Dungeon1Scene");
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

    // Load a random room
    m_dungeonRoom.LoadTiles(roomFiles[randomIndex]);
}