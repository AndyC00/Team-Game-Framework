#include "tile.h"

Tile::Tile() : m_pSprite(nullptr), m_solid(false), m_x(0), m_y(0) {}

Tile::Tile(Sprite* sprite, bool solid, int x, int y)
    : m_pSprite(sprite), m_solid(solid), m_x(x), m_y(y) {}

Tile::~Tile() {
    // Clean up resources if necessary
}

void Tile::Draw(Renderer& renderer) {
    if (m_pSprite) {
        // Manually set the sprite's position before drawing
        m_pSprite->SetX(m_x);
        m_pSprite->SetY(m_y);
        m_pSprite->Draw(renderer);
    }
}

bool Tile::IsSolid() const {
    return m_solid;
}