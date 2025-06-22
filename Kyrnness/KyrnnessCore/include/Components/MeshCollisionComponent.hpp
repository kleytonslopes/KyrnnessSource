#pragma once
#ifndef KYRNNESS_MESH_COLLISION_COMPONENT_HPP
#define KYRNNESS_MESH_COLLISION_COMPONENT_HPP

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

#endif // KYRNNESS_MESH_COLLISION_COMPONENT_HPP
