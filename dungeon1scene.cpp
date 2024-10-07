#include "dungeon1scene.h"
#include "renderer.h"
#include "imgui/imgui.h"

Dungeon1Scene::Dungeon1Scene() : m_dungeonRoom()
{
}

Dungeon1Scene::~Dungeon1Scene()
{
}

bool Dungeon1Scene::Initialise(Renderer& renderer)
{
    // Initialize the dungeon room
    if (!m_dungeonRoom.CreateFromJSON("assets/room_data.json"))
    {
        std::cerr << "Failed to load dungeon room!" << std::endl;
        return false;
    }

    return true;
}

void Dungeon1Scene::Process(float deltaTime, InputSystem& inputSystem)
{
    // Process any logic related to the dungeon room or player here
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
