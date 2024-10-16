#pragma once
#include "scene.h"
#include "dungeonroom.h"
#include "ladder.h"

class Dungeon1Scene : public Scene {
public:
    Dungeon1Scene();
    virtual ~Dungeon1Scene();

    virtual bool Initialise(Renderer& renderer) override;
    virtual void Process(float deltaTime, InputSystem& inputSystem) override;
    virtual void Draw(Renderer& renderer) override;
    virtual void DebugDraw() override;

    virtual void NewRoom();
    virtual void SpawnLadder();

private:
    DungeonRoom m_dungeonRoom; // DungeonRoom instance
    Ladder m_ladder; // Ladder instance
};
