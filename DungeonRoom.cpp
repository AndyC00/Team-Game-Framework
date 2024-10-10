#include "DungeonRoom.h"
#include <iostream>

const int TILE_SIZE = 64; // Updated tile size
const int TILE_FLOOR = 0;
const int TILE_WALL = 1;

int tilemap[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

DungeonRoom::DungeonRoom()
{
}

DungeonRoom::~DungeonRoom()
{
}

void DungeonRoom::LoadTiles()
{
    // Load the textures for floor and wall tiles
    Texture* floorTexture = new Texture();
    floorTexture->Initialise("Sprites\\floor.png"); // Path to your floor texture
    m_floorSprite.Initialise(*floorTexture);

    Texture* wallTexture = new Texture();
    wallTexture->Initialise("Sprites\\wall.png"); // Path to your wall texture
    m_wallSprite.Initialise(*wallTexture);
}

void DungeonRoom::Draw(Renderer& renderer)
{
    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            int tileType = tilemap[y][x];

            if (tileType == TILE_FLOOR)
            {
                m_floorSprite.SetX(x * TILE_SIZE);
                m_floorSprite.SetY(y * TILE_SIZE);
                m_floorSprite.SetScale(2.0f);
                m_floorSprite.Draw(renderer);
            }
            else if (tileType == TILE_WALL)
            {
                m_wallSprite.SetX(x * TILE_SIZE);
                m_wallSprite.SetY(y * TILE_SIZE);
                m_wallSprite.SetScale(2.0f);
                m_wallSprite.Draw(renderer);
            }
            else
            {
                std::cerr << "Unknown tile type at (" << x << ", " << y << ")" << std::endl;
            }
        }
    }
}
