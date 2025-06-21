#pragma once
#ifndef KYRNNESS_BOX_COLLISION_COMPONENT_HPP
#define KYRNNESS_BOX_COLLISION_COMPONENT_HPP

#include "Components/CollisionComponent.hpp"

class UBoxCollisionComponent : public UCollisionComponent
{
	using Super = UCollisionComponent;

public:
	UBoxCollisionComponent() = default;

	UBoxCollisionComponent(const UBoxCollisionComponent&) = delete;
	UBoxCollisionComponent& operator=(const UBoxCollisionComponent&) = delete;
	UBoxCollisionComponent(UBoxCollisionComponent&&) = delete;
	UBoxCollisionComponent& operator=(UBoxCollisionComponent&&) = delete;

	virtual nlohmann::json GetJsonData() override;
};

#endif // KYRNNESS_BOX_COLLISION_COMPONENT_HPP
