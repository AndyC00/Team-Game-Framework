#include "dungeon1scene.h"
#include "renderer.h"
#include "imgui/imgui.h"
#include <iostream> // Include iostream for std::cerr and std::endl
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
    m_dungeonRoom.LoadTiles(); // Use LoadTiles instead of CreateFromJSON

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
