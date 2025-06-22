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

static TMap<std::string, FMeshAsset> m_MeshCache;
TMap<std::string, FAssetEntry> UAssetManager::s_AssetMap;
TMap<std::string, FAssetTexture> UAssetManager::m_TextureLoaded;
std::ifstream UAssetManager::s_AssetFile;
/*
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
		hasUBOX = mesh.name.rfind("UBOX_", 0) == 0; // come�a com "UBOX_"

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
*/

void UAssetManager::LoadMeshAsset(const std::string& meshFilePath, FMeshAsset& meshAsset)
{
	if (m_MeshCache.find(meshFilePath) != m_MeshCache.end())
	{
		meshAsset = m_MeshCache[meshFilePath];
		return;
	}

	// Carregar bin�rio do .glb da mem�ria
	std::vector<uint8_t> meshData;
	try
	{
		meshData = LoadAssetRaw(meshFilePath);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Falha ao carregar mesh asset: " << meshFilePath << " | Erro: " << e.what() << std::endl;
		return;
	}

	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	std::string err, warn;

	bool ret = loader.LoadBinaryFromMemory(&model, &err, &warn, meshData.data(), meshData.size(), "");

	if (!warn.empty()) std::cout << "Warning: " << warn << std::endl;
	if (!err.empty()) std::cerr << "Error: " << err << std::endl;
	if (!ret) {
		std::cerr << "Failed to load glTF model from memory: " << meshFilePath << std::endl;
		return;
	}

	// O resto do c�digo (parse dos vertices, indices e texturas) permanece igual:
	bool hasUBOX = false;

	for (const auto& mesh : model.meshes)
	{
		hasUBOX = mesh.name.rfind("UBOX_", 0) == 0;

		for (const auto& primitive : mesh.primitives) {
			if (primitive.mode != TINYGLTF_MODE_TRIANGLES) continue;

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
	auto rawData = LoadAssetRaw(jsonFilePath);
	std::string jsonString(rawData.begin(), rawData.end());
	return nlohmann::json::parse(jsonString);
	/*std::ifstream sceneFile(jsonFilePath);

	if (!sceneFile)
		ThrowRuntimeError("Failed to open json file: " + jsonFilePath);

	nlohmann::json j;
	sceneFile >> j;

	if (j.is_object())
	{
		return j;
	}

	return nlohmann::json();*/
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

GLuint UAssetManager::LoadTextureOpenGL(const std::string& filePath, bool isUI)
{
	auto it = m_TextureLoaded.find(filePath);
	if (it != m_TextureLoaded.end())
	{
		return it->second.m_TextureId;
	}

	// Carregar o arquivo de textura da mem�ria
	std::vector<uint8_t> textureData;
	try
	{
		textureData = LoadAssetRaw(filePath);
	}
	catch (const std::exception& e)
	{
		printf("Failed to load texture asset: %s | Error: %s\n", filePath.c_str(), e.what());
		return 0;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load_from_memory(textureData.data(), static_cast<int>(textureData.size()), &width, &height, &nrChannels, 4);

	if (!data)
	{
		printf("Failed to decode texture from memory: %s\n", filePath.c_str());
		return 0;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (!isUI)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (isUI)
	{

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	stbi_image_free(data);

	FAssetTexture newLoadedTextureAsset;
	newLoadedTextureAsset.m_Path = filePath;
	newLoadedTextureAsset.m_TextureId = textureID;

	m_TextureLoaded[filePath] = newLoadedTextureAsset;

	return textureID;
}

void UAssetManager::InitializeGData(const std::string& gdataFilePath)
{
	// Ler tudo pra mem�ria
	std::ifstream file(gdataFilePath, std::ios::binary | std::ios::ate);
	if (!file)
	{
		throw std::runtime_error("Falha ao abrir GData file");
	}

	size_t fileSize = file.tellg();
	file.seekg(0);
	s_GDataFile.resize(fileSize);
	file.read(reinterpret_cast<char*>(s_GDataFile.data()), fileSize);

	// Parse do Header
	struct GDataHeader
	{
		char Magic[4];
		uint32_t Version;
		uint32_t FileCount;
		uint32_t DataOffset;
	};

	GDataHeader* header = reinterpret_cast<GDataHeader*>(s_GDataFile.data());
	if (std::string(header->Magic, 4) != "KGPK")
	{
		throw std::runtime_error("GData: Header inv�lido");
	}

	size_t offset = sizeof(GDataHeader);

	for (uint32_t i = 0; i < header->FileCount; i++)
	{
		uint32_t pathSize = *reinterpret_cast<uint32_t*>(&s_GDataFile[offset]);
		offset += sizeof(uint32_t);

		std::string path(reinterpret_cast<char*>(&s_GDataFile[offset]), pathSize);
		offset += pathSize;

		uint32_t nameSize = *reinterpret_cast<uint32_t*>(&s_GDataFile[offset]);
		offset += sizeof(uint32_t);

		std::string name(reinterpret_cast<char*>(&s_GDataFile[offset]), nameSize);
		offset += nameSize;

		FAssetEntry entry;
		entry.Name = name;

		entry.Offset = *reinterpret_cast<uint32_t*>(&s_GDataFile[offset]);
		offset += sizeof(uint32_t);

		entry.UncompressedSize = *reinterpret_cast<uint32_t*>(&s_GDataFile[offset]);
		offset += sizeof(uint32_t);

		entry.CompressedSize = *reinterpret_cast<uint32_t*>(&s_GDataFile[offset]);
		offset += sizeof(uint32_t);

		entry.CRC32 = *reinterpret_cast<uint32_t*>(&s_GDataFile[offset]);
		offset += sizeof(uint32_t);

		s_AssetMap[path] = entry;
	}

	s_AssetFile.open(gdataFilePath, std::ios::binary);
	if (!s_AssetFile)
	{
		throw std::runtime_error("Falha ao reabrir o .gdata para leitura de conte�do.");
	}
}

void UAssetManager::LoadAssetMap(const std::string& path)
{
	std::ifstream inFile(path, std::ios::binary);
	if (!inFile)
	{
		ThrowRuntimeError("Falha ao abrir o arquivo de assets: " + path);
		return;
	}

	char magic[4];
	inFile.read(magic, 4);
	if (std::string(magic, 4) != "KGPK")
	{
		ThrowRuntimeError("Arquivo .gdata inv�lido: " + path);
		return;
	}

	uint32_t version;
	uint32_t numFiles;
	uint32_t dataOffset;

	inFile.read(reinterpret_cast<char*>(&version), sizeof(uint32_t));
	inFile.read(reinterpret_cast<char*>(&numFiles), sizeof(uint32_t));
	inFile.read(reinterpret_cast<char*>(&dataOffset), sizeof(uint32_t));

	s_AssetMap.clear();

	for (uint32_t i = 0; i < numFiles; ++i)
	{
		uint32_t pathSize;
		inFile.read(reinterpret_cast<char*>(&pathSize), sizeof(uint32_t));

		std::string relativePath(pathSize, '\0');
		inFile.read(relativePath.data(), pathSize);

		FAssetEntry entry;

		inFile.read(reinterpret_cast<char*>(&entry.Offset), sizeof(uint32_t));
		inFile.read(reinterpret_cast<char*>(&entry.UncompressedSize), sizeof(uint32_t));
		inFile.read(reinterpret_cast<char*>(&entry.CompressedSize), sizeof(uint32_t));
		inFile.read(reinterpret_cast<char*>(&entry.CRC32), sizeof(uint32_t));

		s_AssetMap[relativePath] = entry;
	}

	s_AssetFile.open(path, std::ios::binary);
	if (!s_AssetFile)
	{
		ThrowRuntimeError("Falha ao reabrir o .gdata para leitura de conte�do.");
	}
}

std::string UAssetManager::LoadText(const std::string& FilePath)
{
	try
	{
		// Primeiro tenta carregar do .kpak
		std::vector<uint8_t> rawData = LoadAssetRaw(FilePath, false);

		if (!rawData.empty())
		{
			rawData.push_back(0); // Garante null-terminado
			return std::string(reinterpret_cast<const char*>(rawData.data()));
		}

	}
	catch (const std::exception&)
	{
		//File From Disk
		std::ifstream file(FilePath, std::ios::binary);
		if (file)
		{
			std::ostringstream buffer;
			buffer << file.rdbuf();
			printf("[AssetManager] Lendo script direto do disco: %s\n", FilePath.c_str());
			return buffer.str();
		}

		printf("[AssetManager] Erro: Asset não encontrado no .kpak nem no disco: %s\n", FilePath.c_str());
		return "";
	}
}

std::vector<uint8_t> UAssetManager::LoadAssetRaw_NoEncryption(const std::string& assetPath, bool bIsBinary)
{
	auto it = s_AssetMap.find(assetPath);
	if (it == s_AssetMap.end())
	{
		ThrowRuntimeError("Asset não encontrado: " + assetPath);
	}

	const FAssetEntry& entry = it->second;

	s_AssetFile.seekg(entry.Offset, std::ios::beg);

	std::vector<uint8_t> compressedData(entry.CompressedSize);
	s_AssetFile.read(reinterpret_cast<char*>(compressedData.data()), entry.CompressedSize);

	std::vector<uint8_t> uncompressedData(entry.UncompressedSize);

	uLongf destLen = static_cast<uLongf>(entry.UncompressedSize);
	int result = uncompress(uncompressedData.data(), &destLen, compressedData.data(), entry.CompressedSize);

	if (result != Z_OK)
	{
		ThrowRuntimeError("Falha ao descomprimir asset: " + assetPath);
	}

	// Verificar CRC
	uint32_t calcCrc = ::crc32(0L, Z_NULL, 0);
	calcCrc = ::crc32(calcCrc, uncompressedData.data(), uncompressedData.size());

	if (calcCrc != entry.CRC32)
	{
		ThrowRuntimeError("CRC inválido para asset: " + assetPath);
	}

	return uncompressedData;
}

std::vector<uint8_t> UAssetManager::LoadAssetRaw_With_XOR(const std::string& assetPath, bool bIsBinary)
{
	auto it = s_AssetMap.find(assetPath);
	if (it == s_AssetMap.end())
	{
		ThrowRuntimeError("Asset não encontrado: " + assetPath);
	}

	const FAssetEntry& entry = it->second;

	std::vector<uint8_t> compressedData(entry.CompressedSize);
	std::copy(s_GDataFile.begin() + entry.Offset,
		s_GDataFile.begin() + entry.Offset + entry.CompressedSize,
		compressedData.begin());

	uint8_t xorKey = static_cast<uint8_t>((entry.CRC32 ^ 0xA5) & 0xFF);
	XORDecrypt(compressedData, xorKey);

	std::vector<uint8_t> uncompressedData(entry.UncompressedSize);
	uLongf destLen = entry.UncompressedSize;

	int result = uncompress(uncompressedData.data(), &destLen, compressedData.data(), entry.CompressedSize);
	if (result != Z_OK)
	{
		ThrowRuntimeError("Falha ao descomprimir asset: " + assetPath);
	}

	return uncompressedData;
}

void UAssetManager::XORDecrypt(std::vector<uint8_t>& data, uint8_t key)
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		data[i] ^= key;
	}
}

