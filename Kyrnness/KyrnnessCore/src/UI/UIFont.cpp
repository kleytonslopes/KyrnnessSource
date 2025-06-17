#include "pch.hpp"
#include "UI/UIFont.hpp"
#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "Runtime/Application.hpp"
#include <entt/entt.hpp>
#include "Components/ShaderOpenGLComponent.hpp"
#include <GL/gl.h>

void UUIFont::Load(const std::string& pngPath, const std::string& csvPath)
{
	int w, h, c;
	unsigned char* data = stbi_load(pngPath.c_str(), &w, &h, &c, STBI_rgb_alpha);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	stbi_image_free(data);

	std::ifstream file(csvPath);
	std::string line;
	while (std::getline(file, line)) {
        int texWidth = w;
        int texHeight = h;

		std::stringstream ss(line);
		char ch;
		int x, y, w, h;
		ss >> ch; ss.ignore();
		ss >> x; ss.ignore();
		ss >> y; ss.ignore();
		ss >> w; ss.ignore();
		ss >> h;
        glyphs[ch] = {
            x / (float)texWidth,
            y / (float)texHeight,
            (x + w) / (float)texWidth,
            (y + h) / (float)texHeight,
            (float)w
        };
	}

	lineHeight = 128.0f;

    std::cout << "Fonte carregada: texture ID = " << texture << ", glyphs = " << glyphs.size() << std::endl;
}

void UUIFont::DrawText(const std::string& text, float x, float y, float scale)
{
    if (vao == 0) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    auto shaderEntities = UApplication::Get().GetEnttRegistry().view<FShaderOpenGLComponent>();
    shaderEntities.each([&](const auto entity, auto& shader)
        {
            if (shader.GetShaderName() == "uiShader")
            {
                shader.Bind();
                shader.SetMatrix4("projection", glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f));
                shader.SetMatrix4("model", glm::mat4(1.0f));
                shader.SetVector4("color", glm::vec4(1.0f));
                shader.SetInt("tex", 0); // texture unit 0
            }

        });

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (char ch : text) {
        if (glyphs.count(ch) == 0) continue;
        const Glyph& g = glyphs[ch];

        float w = g.width * scale * size;
        float h = lineHeight * scale * size;

        float vertices[] = {
            x,     y,     g.u0, g.v0,
            x + w, y,     g.u1, g.v0,
            x + w, y + h, g.u1, g.v1,
            x,     y + h, g.u0, g.v1
        };

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        x += w;
    }

    glBindVertexArray(0);
}
float UUIFont::CalculateTextWidth(const std::string& text)
{
    float width = 0.0f;
    for (char ch : text) {
        if (glyphs.count(ch)) {
            width += glyphs.at(ch).width;
        }
    }
    return width;
}
float UUIFont::GetLineHeight() const
{
    return lineHeight;
}
//