#pragma once
#include "Texture.h"
#include "Sprite.h"
#include "Renderer.h"
#include <string>
#include <vector>

class DungeonRoom
{
public:
    DungeonRoom();
    ~DungeonRoom();

    void LoadTiles();
    void LoadTilemapFromJSON(const std::string& filename);
    void Draw(Renderer& renderer);

    bool IsTilePassable(int x, int y); // Checks if a tile is passable based on tile type
    bool IsCollisionAt(float x, float y); // Checks for collision at world coordinates

private:
    Sprite m_floorSprite;
    Sprite m_wallSprite;

    int tilemap[10][10]; // The tilemap will store both the tile type and collision information
};
