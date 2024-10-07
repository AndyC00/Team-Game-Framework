#ifndef  DUNGEONROOM_H 
#define  DUNGEONROOM_H  

#include "sprite.h"
#include "json.hpp" // Include the nlohmann/json header
#include "SpriteSheet.h"
#include <fstream>  // For file reading
#include <iostream>


using json = nlohmann::json;

class DungeonRoom {
public:
    DungeonRoom();
    ~DungeonRoom();

    int GetIndex(int x, int y);
    bool GetSolid(int x, int y);

    bool CreateFromJSON(const std::string& filename);
    void Draw(Renderer& renderer);

private:
    int nWidth;
    int nHeight;
    int roomID;

    int* m_indices;
    bool* m_solids;

    SpriteSheet* m_pSpriteSheet; // Pointer to the SpriteSheet

    void LoadTiles(); // Load the sprite sheet for the tiles
};

#endif //   DUNGEONROOM_H 