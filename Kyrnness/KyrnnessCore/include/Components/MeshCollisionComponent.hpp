#pragma once
#ifndef KYRNNESS_MESH_COLLISION_COMPONENT_HPP
#define KYRNNESS_MESH_COLLISION_COMPONENT_HPP

#include "Components/CollisionComponent.hpp"

#include "Content/MeshAsset.hpp"

class FMeshCollisionComponent : public FCollisionComponent
{
	using Super = UComponent;
public:
	FMeshCollisionComponent() = default;
	virtual ~FMeshCollisionComponent() = default;

	FMeshCollisionComponent(const FMeshCollisionComponent&) = delete;
	FMeshCollisionComponent& operator=(const FMeshCollisionComponent&) = delete;
	FMeshCollisionComponent(FMeshCollisionComponent&&) = delete;
	FMeshCollisionComponent& operator=(FMeshCollisionComponent&&) = delete;


	void SetMeshAsset(const FMeshAsset& meshAsset);

	virtual nlohmann::json GetJsonData() override;
};

#endif // KYRNNESS_MESH_COLLISION_COMPONENT_HPP
