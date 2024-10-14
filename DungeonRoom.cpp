#include "DungeonRoom.h"
#include <iostream>
#include <fstream>
#include "json.hpp" // Include the JSON library

using json = nlohmann::json;

const int TILE_SIZE = 64;
const int TILE_FLOOR = 0;
const int TILE_WALL = 1;

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

    // Load the tilemap from a JSON file
    LoadTilemapFromJSON("Rooms\\Room1.json");
}

void DungeonRoom::LoadTilemapFromJSON(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    json roomData;
    file >> roomData;

    if (roomData.contains("tilemap"))
    {
        for (int y = 0; y < 10; ++y)
        {
            for (int x = 0; x < 10; ++x)
            {
                tilemap[y][x] = roomData["tilemap"][y][x];
            }
        }
    }
    else
    {
        std::cerr << "No tilemap found in JSON file!" << std::endl;
    }
}

bool DungeonRoom::IsTilePassable(int x, int y)
{
    if (x < 0 || x >= 10 || y < 0 || y >= 10)
    {
        return false; // Out of bounds, treat as non-passable.
    }

    // Check if the tile is a wall or not. Only walls are non-passable.
    return (tilemap[y][x] != TILE_WALL);
}

bool DungeonRoom::IsCollisionAt(float x, float y)
{
    int tileX = static_cast<int>(x / TILE_SIZE);
    int tileY = static_cast<int>(y / TILE_SIZE);

    // Return true if the tile is a wall (i.e., a collision occurs).
    return !IsTilePassable(tileX, tileY);
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
