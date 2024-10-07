#ifndef  TILE_H   
#define  TILE_H 
#include "sprite.h"

class Tile {
public:
    Tile();
    Tile(Sprite* sprite, bool solid, int x, int y);
    ~Tile();

    void Draw(Renderer& renderer); // Method to draw the tile on the screen
    bool IsSolid() const;

private:
    Sprite* m_pSprite;
    bool m_solid;
    int m_x;
    int m_y;
};
#endif //   TILE_H 