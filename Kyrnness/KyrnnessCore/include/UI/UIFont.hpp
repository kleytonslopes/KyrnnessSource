#pragma once
#ifndef KYRNESS_UI_UIFONT_HPP
#define KYRNESS_UI_UIFONT_HPP

#include "Core/Core.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

struct FGlyphInfo
{
    glm::vec2 Size;
    glm::vec2 Bearing;
    float Advance;
    glm::vec4 UV; // u0, v0, u1, v1
};

class UUIFont
{
public:
    UUIFont(const std::string& filePath, int pixelSize);
    ~UUIFont();

    GLuint GetTextureId() const { return m_TextureID; }
    const FGlyphInfo* GetGlyph(char c) const;

private:
    void LoadFont(const std::string& filePath, int pixelSize);

    std::map<char, FGlyphInfo> m_Glyphs;
    GLuint m_TextureID = 0;
    int m_AtlasWidth = 0;
    int m_AtlasHeight = 0;
};

#endif// KYRNESS_UI_UIFONT_HPP
