#include "spritesheet.h"

SpriteSheet::SpriteSheet()
    : m_pTexture(nullptr), m_spriteWidth(0), m_spriteHeight(0),
    m_sheetWidth(0), m_sheetHeight(0), m_spriteCountX(0), m_spriteCountY(0) {}

SpriteSheet::~SpriteSheet() {}

bool SpriteSheet::Initialise(Texture& texture, int spriteWidth, int spriteHeight) {
    m_pTexture = &texture;
    m_spriteWidth = spriteWidth;
    m_spriteHeight = spriteHeight;

    m_sheetWidth = m_pTexture->GetWidth();
    m_sheetHeight = m_pTexture->GetHeight();

    m_spriteCountX = m_sheetWidth / m_spriteWidth;
    m_spriteCountY = m_sheetHeight / m_spriteHeight;

    return (m_spriteCountX > 0 && m_spriteCountY > 0);
}

void SpriteSheet::GetSpriteUV(int index, float& uMin, float& vMin, float& uMax, float& vMax) const {
    int spriteX = index % m_spriteCountX;
    int spriteY = index / m_spriteCountX;

    uMin = (spriteX * m_spriteWidth) / static_cast<float>(m_sheetWidth);
    vMin = (spriteY * m_spriteHeight) / static_cast<float>(m_sheetHeight);
    uMax = ((spriteX + 1) * m_spriteWidth) / static_cast<float>(m_sheetWidth);
    vMax = ((spriteY + 1) * m_spriteHeight) / static_cast<float>(m_sheetHeight);
}

int SpriteSheet::GetSpriteCountX() const {
    return m_spriteCountX;
}

int SpriteSheet::GetSpriteCountY() const {
    return m_spriteCountY;
}

int SpriteSheet::GetSpriteWidth() const {
    return m_spriteWidth;
}

int SpriteSheet::GetSpriteHeight() const {
    return m_spriteHeight;
}
