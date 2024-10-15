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

    void LoadTiles(const std::string& filename);
    void LoadTilemapFromJSON(const std::string& filename);
    void Draw(Renderer& renderer);
    void OnTileClicked(int mouseX, int mouseY);

    bool IsTilePassable(int x, int y);
    bool IsCollisionAt(float x, float y);

private:
    Sprite m_floorSprite;
    Sprite m_wallSprite;

    std::vector<std::vector<int>> tilemap; // The tilemap will store both the tile type and collision information
};
