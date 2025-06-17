#pragma once
#ifndef KYRNNESS_MESH_COLLISION_COMPONENT_HPP
#define KYRNNESS_MESH_COLLISION_COMPONENT_HPP

#include "Components/CollisionComponent.hpp"

#include "Content/MeshAsset.hpp"

class FMeshCollisionComponent : public FCollisionComponent
{
public:
	FMeshCollisionComponent() = default;
	virtual ~FMeshCollisionComponent() = default;

	FMeshCollisionComponent(const FMeshCollisionComponent&) = delete;
	FMeshCollisionComponent& operator=(const FMeshCollisionComponent&) = delete;
	FMeshCollisionComponent(FMeshCollisionComponent&&) = delete;
	FMeshCollisionComponent& operator=(FMeshCollisionComponent&&) = delete;

	void Initialize() override;

	//void SetMass(float mass) override;
	//void SetBoundBox(const glm::vec3& boundBox) override;

	void SetMeshAsset(const FMeshAsset& meshAsset);
};

#endif // KYRNNESS_MESH_COLLISION_COMPONENT_HPP
