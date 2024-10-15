#include "ladder.h"
#include "renderer.h"
#include <iostream>

Ladder::Ladder()
    : m_currentRoom(nullptr)
{
}

Ladder::~Ladder()
{
}

bool Ladder::Initialise(Renderer& renderer, const char* spritePath, DungeonRoom* currentRoom, const std::string& nextRoomFilename)
{
    m_currentRoom = currentRoom;
    m_nextRoomFilename = nextRoomFilename;

    return Entity::Initialise(renderer, spritePath); // Initialize the sprite and other properties
}

void Ladder::Process(float deltaTime)
{
    // You can handle any specific behavior for this entity here
    Entity::Process(deltaTime); // Call base class method for position update
}

bool Ladder::CheckCollisionWithPlayer(Entity& player)
{
    if (IsCollidingWith(player)) // If the player collides with this entity
    {
        std::cout << "Player collided with RoomTriggerEntity, loading new room..." << std::endl;

        // Load the next room
        if (m_currentRoom)
        {
            m_currentRoom->LoadTilemapFromJSON(m_nextRoomFilename);
        }

        return true;
    }

    return false;
}
