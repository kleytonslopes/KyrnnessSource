#pragma once
#ifndef KYRNNESS_TERRAIN_HPP
#define KYRNNESS_TERRAIN_HPP

#include "Core/Core.hpp"
#include "Core/Vertex.hpp"

class UTerrain
{
public:
	UTerrain() = default;
	virtual ~UTerrain() = default;

	UTerrain(const UTerrain&) = delete;
	UTerrain& operator=(const UTerrain&) = delete;
	UTerrain(UTerrain&&) = delete;
	UTerrain& operator=(UTerrain&&) = delete;

	static void GenerateTerrain(float noise, float amplitude, int width, int height, float tileSize, std::vector<Vertex>& outVertices, std::vector<uint32>& outIndices);
};
#endif// KYRNNESS_TERRAIN_HPP
