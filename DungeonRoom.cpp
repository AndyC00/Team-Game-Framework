#include "DungeonRoom.h"
#include <iostream>
#include <fstream>
#include "json.hpp" // Include the JSON library

using json = nlohmann::json;

const int TILE_SIZE = 32;
const int TILE_FLOOR = 0;
const int TILE_WALL = 1;

// Set the tilemap size as a constant
const int TILEMAP_SIZE = 20; // You can change this to any value

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
        int rows = roomData["tilemap"].size();
        int cols = roomData["tilemap"][0].size();

        // Resize the tilemap to fit the data
        tilemap.resize(rows);
        for (int y = 0; y < rows; ++y)
        {
            tilemap[y].resize(cols);
            for (int x = 0; x < cols; ++x)
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
    if (x < 0 || x >= TILEMAP_SIZE || y < 0 || y >= TILEMAP_SIZE)
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

void DungeonRoom::OnTileClicked(int mouseX, int mouseY)
{
    // Convert mouse coordinates to tile coordinates
    int tileX = mouseX / TILE_SIZE;
    int tileY = mouseY / TILE_SIZE;

    // Check if the tile is passable
    if (IsTilePassable(tileX, tileY))
    {
        std::cout << "Tile at (" << tileX << ", " << tileY << ") is passable." << std::endl;
    }
    else
    {
        std::cout << "Tile at (" << tileX << ", " << tileY << ") is not passable." << std::endl;
    }
}

void DungeonRoom::Draw(Renderer& renderer)
{
    int rows = tilemap.size();
    int cols = tilemap[0].size(); // Assumes at least one row

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            int tileType = tilemap[y][x];

            if (tileType == TILE_FLOOR)
            {
                m_floorSprite.SetX(x * TILE_SIZE);
                m_floorSprite.SetY(y * TILE_SIZE);
                m_floorSprite.SetScale(1.0f);
                m_floorSprite.Draw(renderer);
            }
            else if (tileType == TILE_WALL)
            {
                m_wallSprite.SetX(x * TILE_SIZE);
                m_wallSprite.SetY(y * TILE_SIZE);
                m_wallSprite.SetScale(1.0f);
                m_wallSprite.Draw(renderer);
            }
            else
            {
                std::cerr << "Unknown tile type at (" << x << ", " << y << ")" << std::endl;
            }
        }
    }
}

