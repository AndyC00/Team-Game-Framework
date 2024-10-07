#pragma once
#include "texture.h"

class SpriteSheet {
public:
    SpriteSheet();
    ~SpriteSheet();

    bool Initialise(Texture& texture, int spriteWidth, int spriteHeight);

    void GetSpriteUV(int index, float& uMin, float& vMin, float& uMax, float& vMax) const;

    int GetSpriteCountX() const;
    int GetSpriteCountY() const;

    int GetSpriteWidth() const;
    int GetSpriteHeight() const;

    Texture* GetTexture() const { return m_pTexture; }

private:
    Texture* m_pTexture;
    int m_spriteWidth;
    int m_spriteHeight;
    int m_sheetWidth;
    int m_sheetHeight;
    int m_spriteCountX;
    int m_spriteCountY;
};
