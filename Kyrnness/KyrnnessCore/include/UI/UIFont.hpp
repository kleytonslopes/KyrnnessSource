#pragma once
#ifndef KYRNESS_UI_UIFONT_HPP
#define KYRNESS_UI_UIFONT_HPP

#include "Core/Core.hpp"

struct Glyph {
    float u0, v0;  // topo-esquerda
    float u1, v1;  // baixo-direita
    float width; // largura do caractere
};

class UUIFont
{
public:
    GLuint texture;
    std::map<char, Glyph> glyphs;
    float lineHeight = 16.0f;
    GLuint vao = 0;
    GLuint vbo = 0;
    float size = 0.16f;


    void Load(const std::string& pngPath, const std::string& csvPath);
    void DrawText(const std::string& text, float x, float y, float scale = 1.0f);

    float CalculateTextWidth(const std::string& text);

    float GetLineHeight() const;
};

#endif// KYRNESS_UI_UIFONT_HPP
