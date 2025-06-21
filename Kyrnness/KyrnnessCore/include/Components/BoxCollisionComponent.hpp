#pragma once
#ifndef KYRNNESS_BOX_COLLISION_COMPONENT_HPP
#define KYRNNESS_BOX_COLLISION_COMPONENT_HPP

#include "Components/CollisionComponent.hpp"

class FBoxCollisionComponent : public FCollisionComponent
{
	using Super = FCollisionComponent;

public:
	FBoxCollisionComponent() = default;

	FBoxCollisionComponent(const FBoxCollisionComponent&) = delete;
	FBoxCollisionComponent& operator=(const FBoxCollisionComponent&) = delete;
	FBoxCollisionComponent(FBoxCollisionComponent&&) = delete;
	FBoxCollisionComponent& operator=(FBoxCollisionComponent&&) = delete;

	virtual nlohmann::json GetJsonData() override;
};

#endif // KYRNNESS_BOX_COLLISION_COMPONENT_HPP
