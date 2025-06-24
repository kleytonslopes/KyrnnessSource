#pragma once
#ifndef KYRNNESS_MESH_ASSET_HPP
#define KYRNNES_MESH_ASSET_HPP

#include "Core/Core.hpp"
#include "Core/Vertex.hpp"

#include "tiny_gltf.h"

struct FMeshAsset
{
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;

	glm::vec3 boundingBoxMin = glm::vec3(FLT_MAX);
	glm::vec3 boundingBoxMax = glm::vec3(-FLT_MAX);

	glm::vec3 BoundingBoxCenter() const
	{
		return (boundingBoxMin + boundingBoxMax) * 0.5f;
	}

	std::vector<tinygltf::Texture> textures;
	std::vector<tinygltf::Image> images;
};

#endif // KYRNNESS_MESH_ASSET_HPP
