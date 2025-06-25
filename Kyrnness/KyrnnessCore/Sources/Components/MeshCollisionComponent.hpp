/*********************************************************************
 *   File: MeshCollisionComponent.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_MESH_COLLISION_COMPONENT_HPP
#define K_MESH_COLLISION_COMPONENT_HPP

#include "Components/CollisionComponent.hpp"

#include "Content/MeshAsset.hpp"

class UMeshCollisionComponent : public UCollisionComponent
{
	using Super = UComponent;
public:
	UMeshCollisionComponent() = default;
	virtual ~UMeshCollisionComponent() = default;

	UMeshCollisionComponent(const UMeshCollisionComponent&) = delete;
	UMeshCollisionComponent& operator=(const UMeshCollisionComponent&) = delete;
	UMeshCollisionComponent(UMeshCollisionComponent&&) = delete;
	UMeshCollisionComponent& operator=(UMeshCollisionComponent&&) = delete;


	void SetMeshAsset(const FMeshAsset& meshAsset);

	virtual nlohmann::json GetJsonData() override;
};

#endif // K_MESH_COLLISION_COMPONENT_HPP
