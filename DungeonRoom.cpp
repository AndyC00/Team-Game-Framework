#include "DungeonRoom.h"
#include "renderer.h"

DungeonRoom::DungeonRoom()
    : nWidth(0), nHeight(0), roomID(0), m_pSpriteSheet(nullptr), m_indices(nullptr), m_solids(nullptr)
{
}

DungeonRoom::~DungeonRoom()
{
    delete[] m_indices;
    delete[] m_solids;
    delete m_pSpriteSheet;
}

int DungeonRoom::GetIndex(int x, int y)
{
    if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
        return m_indices[y * nWidth + x];
    return -1; // Invalid index
}

bool DungeonRoom::GetSolid(int x, int y)
{
    if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
        return m_solids[y * nWidth + x];
    return false;
}

bool DungeonRoom::CreateFromJSON(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    json j;
    file >> j;

    // Parse room data
    roomID = j["roomID"];
    nWidth = j["width"];
    nHeight = j["height"];

    // Parse spritesheet data
    std::string spritesheetFile = j["spritesheet"]["file"];
    int tileWidth = j["spritesheet"]["tileWidth"];
    int tileHeight = j["spritesheet"]["tileHeight"];

    // Load the spritesheet
    Texture* tileTexture = new Texture();
    tileTexture->Initialise(spritesheetFile.c_str());

    m_pSpriteSheet = new SpriteSheet();
    m_pSpriteSheet->Initialise(*tileTexture, tileWidth, tileHeight);

    // Allocate memory for indices and solids
    m_indices = new int[nWidth * nHeight];
    m_solids = new bool[nWidth * nHeight];

    // Copy the data from JSON to arrays
    for (int i = 0; i < nWidth * nHeight; ++i)
    {
        m_indices[i] = j["indices"][i];
        m_solids[i] = j["solids"][i];
    }

    return true;
}


void DungeonRoom::LoadTiles()
{
    // Load the sprite sheet
    Texture* tileTexture = new Texture();
    tileTexture->Initialise("assets/tileset.png"); // Path to your tileset texture

    m_pSpriteSheet = new SpriteSheet();
    m_pSpriteSheet->Initialise(*tileTexture, 32, 32); // Assuming each tile is 32x32 pixels
}

void DungeonRoom::Draw(Renderer& renderer)
{
    for (int y = 0; y < nHeight; ++y)
    {
        for (int x = 0; x < nWidth; ++x)
        {
            int tileIndex = GetIndex(x, y);

            // Get the UV coordinates for this tile
            float uMin, vMin, uMax, vMax;
            m_pSpriteSheet->GetSpriteUV(tileIndex, uMin, vMin, uMax, vMax);

            // Draw the tile using the renderer's fixed-function pipeline
            renderer.DrawTile(x * 32, y * 32, 32, 32, uMin, vMin, uMax, vMax, m_pSpriteSheet->GetTexture());
        }
    }
}
