#include "DungeonRoom.h"

DungeonRoom::DungeonRoom() : nWidth(0), nHeight(0), roomID(0), pSprite(nullptr)
{
}

DungeonRoom::~DungeonRoom()
{
    delete[] m_indices;
    delete[] m_solids;
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