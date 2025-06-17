#include "pch.hpp"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Core/AssetManager.hpp"
#include "Core/Json.hpp"
#include "Core/Vertex.hpp"
#include "GameFramework/Scene.hpp"

#include "glm/glm.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

static std::unordered_map<std::string, FMeshAsset> m_MeshCache;

void UAssetManager::LoadMeshAsset(const std::string& meshFilePath, FMeshAsset& meshAsset)
{
	if (m_MeshCache.find(meshFilePath) != m_MeshCache.end())
	{
		meshAsset = m_MeshCache[meshFilePath];
		return;
	}

	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	std::string err, warn;

	bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, meshFilePath);

	if (!warn.empty()) std::cout << "Warning: " << warn << std::endl;
	if (!err.empty()) std::cerr << "Error: " << err << std::endl;
	if (!ret) {
		std::cerr << "Failed to load glTF model" << std::endl;
		return;
	}

	bool hasUBOX = false;

	for (const auto& mesh : model.meshes)
	{
		hasUBOX = mesh.name.rfind("UBOX_", 0) == 0; // começa com "UBOX_"

		for (const auto& primitive : mesh.primitives) {
			if (primitive.mode != TINYGLTF_MODE_TRIANGLES) {
				std::cerr << "Primitive mode nao suportado: " << primitive.mode << std::endl;
				continue;
			}

			const auto& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
			const auto& posView = model.bufferViews[posAccessor.bufferView];
			const auto& posBuffer = model.buffers[posView.buffer];
			size_t posStride = posAccessor.ByteStride(posView);
			const unsigned char* posData = posBuffer.data.data() + posView.byteOffset + posAccessor.byteOffset;

			const float* texData = nullptr;
			size_t texStride = 0;
			bool hasUV = primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end();
			if (hasUV) {
				const auto& texAccessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
				const auto& texView = model.bufferViews[texAccessor.bufferView];
				const auto& texBuffer = model.buffers[texView.buffer];
				texStride = texAccessor.ByteStride(texView);
				texData = reinterpret_cast<const float*>(texBuffer.data.data() + texView.byteOffset + texAccessor.byteOffset);
			}

			size_t vertexOffset = meshAsset.vertices.size();
			for (size_t i = 0; i < posAccessor.count; i++) {
				const float* position = reinterpret_cast<const float*>(posData + i * posStride);

				Vertex v{};
				v.position = glm::vec3(position[0], position[1], position[2]);
				v.color = glm::vec3(1.0f);

				if (hasUBOX)
				{
					meshAsset.boundingBoxMin = glm::min(meshAsset.boundingBoxMin, v.position);
					meshAsset.boundingBoxMax = glm::max(meshAsset.boundingBoxMax, v.position);
				}

				if (hasUV) {
					const float* tex = reinterpret_cast<const float*>(reinterpret_cast<const unsigned char*>(texData) + i * texStride);
					v.uv = glm::vec2(tex[0], tex[1]);
				}
				else {
					v.uv = glm::vec2(0.0f);
				}

				if (!hasUBOX)
					meshAsset.vertices.push_back(v);
			}

			if (!hasUBOX && primitive.indices >= 0) {
				const auto& indexAccessor = model.accessors[primitive.indices];
				const auto& indexView = model.bufferViews[indexAccessor.bufferView];
				const auto& indexBuffer = model.buffers[indexView.buffer];

				const void* dataPtr = indexBuffer.data.data() + indexView.byteOffset + indexAccessor.byteOffset;

				switch (indexAccessor.componentType) {
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
					const uint8_t* buf = static_cast<const uint8_t*>(dataPtr);
					for (size_t i = 0; i < indexAccessor.count; ++i)
						meshAsset.indices.push_back(vertexOffset + static_cast<uint32_t>(buf[i]));
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
					const uint16_t* buf = static_cast<const uint16_t*>(dataPtr);
					for (size_t i = 0; i < indexAccessor.count; ++i)
						meshAsset.indices.push_back(vertexOffset + static_cast<uint32_t>(buf[i]));
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
					const uint32_t* buf = static_cast<const uint32_t*>(dataPtr);
					for (size_t i = 0; i < indexAccessor.count; ++i)
						meshAsset.indices.push_back(vertexOffset + buf[i]);
					break;
				}
				default:
					std::cerr << "Tipo de Indice nao suportado: " << indexAccessor.componentType << std::endl;
					break;
				}
			}
		}
	}

	if (!hasUBOX)
	{
		for (const Vertex& v : meshAsset.vertices)
		{
			meshAsset.boundingBoxMin = glm::min(meshAsset.boundingBoxMin, v.position);
			meshAsset.boundingBoxMax = glm::max(meshAsset.boundingBoxMax, v.position);
		}
	}

	// Carregar texturas dos materiais
	for (const auto& material : model.materials) {
		const auto& baseColor = material.pbrMetallicRoughness.baseColorTexture;

		if (baseColor.index >= 0) {
			const tinygltf::Texture& texture = model.textures[baseColor.index];
			const tinygltf::Image& image = model.images[texture.source];

			meshAsset.textures.push_back(texture);
			meshAsset.images.push_back(image);
		}
	}

	m_MeshCache[meshFilePath] = meshAsset;
}

TScene UAssetManager::LoadScene(const std::string& sceneFilePath)
{
	TScene sceneLoaded{};

	std::ifstream sceneFile(sceneFilePath);

	if (!sceneFile)
		ThrowRuntimeError("Failed to open scene file: " + sceneFilePath);

	nlohmann::json j;
	sceneFile >> j;

	FJson::FromJson(j, sceneLoaded);

	return sceneLoaded;
}

nlohmann::json UAssetManager::LoadJson(const std::string& jsonFilePath)
{
	std::ifstream sceneFile(jsonFilePath);

	if (!sceneFile)
		ThrowRuntimeError("Failed to open json file: " + jsonFilePath);

	nlohmann::json j;
	sceneFile >> j;

	if (j.is_object())
	{
		return j;
	}

	return nlohmann::json();
}

void UAssetManager::SaveJson(const std::string& jsonFilePath, const nlohmann::json& jsonData)
{
	std::ofstream sceneFile(jsonFilePath);

	if (!sceneFile)
		ThrowRuntimeError("Failed to open json file for writing: " + jsonFilePath);

	sceneFile << std::setw(4) << jsonData << std::endl;

	if (!sceneFile.good())
		ThrowRuntimeError("Failed to write json data to file: " + jsonFilePath);
}

GLuint UAssetManager::LoadTextureOpenGL(const std::string& filePath)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 4);
	if (!data)
	{
		printf("Failed to load texture: %s\n", filePath);
		return 0;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	return textureID;
}
