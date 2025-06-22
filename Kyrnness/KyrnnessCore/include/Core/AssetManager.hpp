#pragma once

#ifndef KYRNNESS_ASSET_MANAGER_HPP
#define KYRNNESS_ASSET_MANAGER_HPP

#include "Core/Core.hpp"
#include "nlohmann/json.hpp"
#include "Content/MeshAsset.hpp"
#include <glad/glad.h>
#include <zlib.h>
#include "Runtime/Packer.hpp"

struct TScene;

struct FAssetEntry
{
	std::string Name;
	uint64_t Offset;
	uint32_t CompressedSize;
	uint32_t UncompressedSize;
	uint32_t CRC32;
};

struct FAssetTexture
{
	std::string m_Name;
	std::string m_Path;
	unsigned int m_TextureId = 0;
};

class UAssetManager
{

public:
	UAssetManager() = default;
	virtual ~UAssetManager() = default;

	UAssetManager(const UAssetManager&) = delete;
	UAssetManager& operator=(const UAssetManager&) = delete;
	UAssetManager(UAssetManager&&) = delete;
	UAssetManager& operator=(UAssetManager&&) = delete;

	static void LoadMeshAsset(const std::string& meshFilePath, FMeshAsset& meshAsset);
	static TScene LoadScene(const std::string& sceneFilePath);
	static nlohmann::json LoadJson(const std::string& jsonFilePath);
	static void SaveJson(const std::string& jsonFilePath, const nlohmann::json& jsonData);
	static GLuint LoadTextureOpenGL(const std::string& filePath, bool isUI = false);
	static void InitializeGData(const std::string& gdataFilePath);

#if defined(ENCRYPTION_XOR)
	static std::vector<uint8_t> LoadAssetRaw(const std::string& assetPath, bool bIsBinary = true)
	{
		return LoadAssetRaw_With_XOR(assetPath, bIsBinary);
	}
#else
	static std::vector<uint8_t> LoadAssetRaw(const std::string& assetPath, bool bIsBinary = true)
	{
		return LoadAssetRaw_NoEncryption(assetPath, bIsBinary);
	}
#endif
	static void LoadAssetMap(const std::string& path);
	
	static std::string LoadText(const std::string& FilePath);
private:
	//inline static TMap<std::string, FAssetEntry> s_AssetMap;
	inline static std::vector<uint8_t> s_GDataFile;
	//static std::ifstream s_AssetFile;

	static TMap<std::string, FAssetEntry> s_AssetMap;
	static std::ifstream s_AssetFile;
	static TMap<std::string, FAssetTexture> m_TextureLoaded;

	static std::vector<uint8_t> LoadAssetRaw_NoEncryption(const std::string& assetPath, bool bIsBinary);
	static std::vector<uint8_t> LoadAssetRaw_With_XOR(const std::string& assetPath, bool bIsBinary);

	static void XORDecrypt(std::vector<uint8_t>& data, uint8_t key);
};

#endif // KYRNNESS_ASSET_MANAGER_HPP
