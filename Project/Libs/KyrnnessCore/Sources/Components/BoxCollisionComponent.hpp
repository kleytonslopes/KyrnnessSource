/*********************************************************************
 *   File: BoxCollisionComponent.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_BOX_COLLISION_COMPONENT_HPP
#define K_BOX_COLLISION_COMPONENT_HPP

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

#endif // K_BOX_COLLISION_COMPONENT_HPP
