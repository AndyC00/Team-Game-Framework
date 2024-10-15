
#pragma once
#include "entity.h"
#include "DungeonRoom.h"

class Ladder : public Entity
{
public:
    Ladder();
    virtual ~Ladder();

    bool Initialise(Renderer& renderer, const char* spritePath, DungeonRoom* currentRoom, const std::string& nextRoomFilename);
    void Process(float deltaTime);
    bool CheckCollisionWithPlayer(Entity& player);

private:
    DungeonRoom* m_currentRoom;
    std::string m_nextRoomFilename;
};
