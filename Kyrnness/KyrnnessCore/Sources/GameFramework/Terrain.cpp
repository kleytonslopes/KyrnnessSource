#include "pch.hpp"
#include "GameFramework/Terrain.hpp"
#include <glm/gtc/noise.hpp>

void UTerrain::GenerateTerrain(float noise, float amplitude, int width, int height, float tileSize, std::vector<Vertex>& outVertices, std::vector<uint32>& outIndices)
{
	outVertices.clear();
	outIndices.clear();

	outVertices.reserve(width * height * 4);
	outIndices.reserve(width * height * 6);

	uint32 index = 0;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// Cálculo da altura por Perlin noise
			auto GetHeight = [&](float fx, float fy) -> float {
				FVector2 p(fx * noise, fy * noise);
				return FMath::PerlinGLM(p) * amplitude;
				};

			FVector topLeft = FVector(x * tileSize, GetHeight(x, y), y * tileSize);
			FVector topRight = FVector((x + 1) * tileSize, GetHeight(x + 1, y), y * tileSize);
			FVector bottomLeft = FVector(x * tileSize, GetHeight(x, y + 1), (y + 1) * tileSize);
			FVector bottomRight = FVector((x + 1) * tileSize, GetHeight(x + 1, y + 1), (y + 1) * tileSize);

			outVertices.push_back({ topLeft,     FVector2(0.0f, 0.0f) });
			outVertices.push_back({ topRight,    FVector2(1.0f, 0.0f) });
			outVertices.push_back({ bottomLeft,  FVector2(0.0f, 1.0f) });
			outVertices.push_back({ bottomRight, FVector2(1.0f, 1.0f) });

			outIndices.push_back(index + 0);
			outIndices.push_back(index + 2);
			outIndices.push_back(index + 1);

			outIndices.push_back(index + 1);
			outIndices.push_back(index + 2);
			outIndices.push_back(index + 3);

			index += 4;
		}
	}
}
