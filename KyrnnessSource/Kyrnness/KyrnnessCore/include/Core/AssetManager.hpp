#pragma once

#ifndef KYRNNESS_ASSET_MANAGER_HPP
#define KYRNNESS_ASSET_MANAGER_HPP

#include "Core/Core.hpp"
#include "nlohmann/json.hpp"
#include "Content/MeshAsset.hpp"


struct TScene;

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


};

#endif // KYRNNESS_ASSET_MANAGER_HPP
