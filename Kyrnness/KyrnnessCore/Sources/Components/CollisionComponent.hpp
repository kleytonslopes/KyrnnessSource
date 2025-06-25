/*********************************************************************
 *   File: CollisionComponent.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_COLLISION_COMPONENT_HPP
#define K_COLLISION_COMPONENT_HPP

#include "Components/Component.hpp"

class UCollisionComponent : public UComponent
{
	using Super = UComponent;

public:
	UCollisionComponent() = default;

	UCollisionComponent(const UCollisionComponent&) = delete;
	UCollisionComponent& operator=(const UCollisionComponent&) = delete;
	UCollisionComponent(UCollisionComponent&&) = delete;
	UCollisionComponent& operator=(UCollisionComponent&&) = delete;

	virtual nlohmann::json GetJsonData() override;

	void SetMass(float mass);
	void SetBoundBox(const glm::vec3& boundBox);

protected:
	float m_Mass = 100.0f;

	glm::vec3 m_BoundBox{ 1.f, 1.f, 1.f };
	glm::vec3 m_InitialInertia{ 0.f, 0.f, 0.f };

	physx::PxRigidStatic* m_BodyStatic = nullptr;
	physx::PxRigidDynamic* m_BodyDynamic = nullptr;

	void OnInitialize() override;
};

#endif // K_COLLISION_COMPONENT_HPP
