#pragma once

#ifndef KYRNNES_CORE_RUNTIME_VERTEX_HPP
#define KYRNNES_CORE_RUNTIME_VERTEX_HPP

#include "Core/Vector.hpp"
#include "vulkan/vulkan.h"

#include <array>


struct Vertex
{
	FVector position;
	FVector2 uv;
	FVector color;

	static VkVertexInputBindingDescription GetBindingDescription() 
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions() 
	{

		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, uv);

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const {
		return position == other.position && color == other.color && uv == other.uv;
	}
};

namespace std {
	template<>
	struct hash<FVector> {
		size_t operator()(const FVector& vec) const noexcept {
			size_t hx = std::hash<float>()(vec.X);
			size_t hy = std::hash<float>()(vec.Y);
			size_t hz = std::hash<float>()(vec.Z);
			size_t combined = hx ^ (hy + 0x9e3779b9 + (hx << 6) + (hx >> 2));
			return combined ^ (hz + 0x9e3779b9 + (combined << 6) + (combined >> 2));
		}
	};

	template<>
	struct hash<FVector2> {
		size_t operator()(const FVector2& vec) const noexcept {
			size_t hx = std::hash<float>()(vec.X);
			size_t hy = std::hash<float>()(vec.Y);
			return hx ^ (hy + 0x9e3779b9 + (hx << 6) + (hx >> 2));
		}
	};

	template<>
	struct hash<Vertex> {
		size_t operator()(const Vertex& vertex) const noexcept {
			size_t h1 = std::hash<FVector>()(vertex.position);
			size_t h2 = std::hash<FVector>()(vertex.color);
			size_t h3 = std::hash<FVector2>()(vertex.uv);

			size_t combined = h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
			return combined ^ (h3 + 0x9e3779b9 + (combined << 6) + (combined >> 2));
		}
	};
}

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

#endif // KYRNNES_CORE_RUNTIME_VERTEX_HPP