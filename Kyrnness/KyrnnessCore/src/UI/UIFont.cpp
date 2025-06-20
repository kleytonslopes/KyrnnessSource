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

	int padding = 2;
	int maxRowHeight = 0;
	int atlasW = 512; // Tamanho fixo inicial (pode fazer dinâmico depois)
	int atlasH = 512;

	std::vector<unsigned char> atlasData(atlasW * atlasH, 0);

	int x = padding;
	int y = padding;
	maxRowHeight = 0;

	for (char c = 32; c < 127; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			continue;

		FT_Bitmap& bmp = face->glyph->bitmap;

		if (x + bmp.width + padding >= atlasW)
		{
			x = padding;
			y += maxRowHeight + padding;
			maxRowHeight = 0;
		}

		for (int row = 0; row < bmp.rows; row++)
		{
			for (int col = 0; col < bmp.width; col++)
			{
				int atlasX = x + col;
				int atlasY = y + row;
				if (atlasX < atlasW && atlasY < atlasH)
					atlasData[atlasY * atlasW + atlasX] = bmp.buffer[row * bmp.pitch + col];
			}
		}

		FGlyphInfo glyph;
		glyph.Size = glm::vec2(bmp.width, bmp.rows);
		glyph.Bearing = glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		glyph.Advance = (float)(face->glyph->advance.x >> 6);

		float u0 = (float)x / atlasW;
		float v0 = 1.0f - (float)(y + bmp.rows) / atlasH;
		float u1 = (float)(x + bmp.width) / atlasW;
		float v1 = 1.0f - (float)y / atlasH;

		glyph.UV = glm::vec4(u0, v0, u1, v1);

		m_Glyphs[c] = glyph;

		x += bmp.width + padding;
		maxRowHeight = std::max(maxRowHeight, (int)bmp.rows);
	}

	m_AtlasWidth = atlasW;
	m_AtlasHeight = atlasH;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasW, atlasH, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	stbi_write_png("FontAtlasDebug.png", atlasW, atlasH, 1, atlasData.data(), atlasW);

}

const FGlyphInfo* UUIFont::GetGlyph(char c) const
{
	auto it = m_Glyphs.find(c);
	if (it != m_Glyphs.end())
		return &it->second;

	return nullptr;
}
//