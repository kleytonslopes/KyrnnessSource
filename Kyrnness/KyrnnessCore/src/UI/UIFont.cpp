#include "pch.hpp"
#include "UI/UIFont.hpp"
#include <ft2build.h>
#include <Core/AssetManager.hpp>
#include FT_FREETYPE_H
#include <stb_image_write.h>

UUIFont::UUIFont(const std::string& filePath, int pixelSize)
{
    LoadFont(filePath, pixelSize);
}

UUIFont::~UUIFont()
{
    if (m_TextureID != 0)
    {
        glDeleteTextures(1, &m_TextureID);
    }
}

void UUIFont::LoadFont(const std::string& filePath, int pixelSize)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        printf("Failed to init FreeType\n");
        return;
    }

    std::vector<uint8_t> fontData = UAssetManager::LoadAssetRaw(filePath);

    FT_Face face;
    FT_Error error = FT_New_Memory_Face(ft, fontData.data(), static_cast<FT_Long>(fontData.size()), 0, &face);
    if (error)
    {
        printf("Failed to load font from memory: %s\n", filePath.c_str());
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, pixelSize);

    // Pré-carregar apenas ASCII 32-126
    int padding = 2;
    int rowHeight = 0;
    int atlasW = 0, atlasH = 0;

    // Primeira passada: calcular tamanho do atlas
    for (char c = 32; c < 127; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        atlasW += face->glyph->bitmap.width + padding;
        rowHeight = std::max(rowHeight, (int)face->glyph->bitmap.rows);
    }

    atlasH = rowHeight + padding;

    m_AtlasWidth = atlasW;
    m_AtlasHeight = atlasH;

    std::vector<unsigned char> atlasData(atlasW * atlasH, 0);

    int offsetX = 0;

    for (char c = 32; c < 127; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        FT_Bitmap& bmp = face->glyph->bitmap;

        // Copiar bitmap pro atlas
        for (int y = 0; y < bmp.rows; y++)
        {
            for (int x = 0; x < bmp.width; x++)
            {
                int atlasX = offsetX + x;
                int atlasY = y;
                atlasData[atlasY * atlasW + atlasX] = bmp.buffer[y * bmp.pitch + x];
            }
        }

        FGlyphInfo glyph;
        glyph.Size = glm::vec2(bmp.width, bmp.rows);
        glyph.Bearing = glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        glyph.Advance = (float)(face->glyph->advance.x >> 6);

        float u0 = (float)offsetX / atlasW;
        float v0 = 1.0f - ((float)bmp.rows / atlasH);
        float u1 = (float)(offsetX + bmp.width) / atlasW;
        float v1 = 1.0f;

        glyph.UV = glm::vec4(u0, v0, u1, v1);

        m_Glyphs[c] = glyph;

        offsetX += bmp.width + padding;
    }

    // Criar textura OpenGL
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasW, atlasH, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

const FGlyphInfo* UUIFont::GetGlyph(char c) const
{
    auto it = m_Glyphs.find(c);
    if (it != m_Glyphs.end())
        return &it->second;

    return nullptr;
}
//