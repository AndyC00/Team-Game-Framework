#ifndef DUNGEONROOM_H
#define DUNGEONROOM_H

#include "renderer.h"
#include "sprite.h"

class DungeonRoom {
public:
    DungeonRoom();
    ~DungeonRoom();

    void LoadTiles(); // Load the textures for the tiles
    void Draw(Renderer& renderer);

private:
    Sprite m_floorSprite; // Sprite for the floor tile
    Sprite m_wallSprite; // Sprite for the wall tile
};

#endif // DUNGEONROOM_H
