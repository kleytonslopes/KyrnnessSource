/*********************************************************************
 *   File: CapsuleComponent.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef K_CAPSULE_COMPONENT_HPP
#define K_CAPSULE_COMPONENT_HPP

#include "Components/Component.hpp"

class UTransformComponent;

class UCapsuleComponent : public UComponent
{
	using Super = UComponent;
public:
	UCapsuleComponent();

	virtual ~UCapsuleComponent() = default;

	UCapsuleComponent(const UCapsuleComponent&) = delete;
	UCapsuleComponent& operator=(const UCapsuleComponent&) = delete;
	UCapsuleComponent(UCapsuleComponent&&) = delete;
	UCapsuleComponent& operator=(UCapsuleComponent&&) = delete;

	virtual nlohmann::json GetJsonData() override;

	void UpdateVerticalMovement(float deltaTime);

	float GetRadius() const { return m_Radius; }
	float GetHeight() const { return m_Height; }
	float GetVerticalVelocity() const { return m_VerticalVelocity; }

	void Move(glm::vec3 direction, float scale);
	void Jump();

	void SetIsJumping(bool bValue) { bIsJumping = bValue; }
	bool GetIsJumping() const { return bIsJumping; }
	bool IsOnGround() const;
	bool CheckGround(float maxDistance, physx::PxRaycastHit& hitOut) const;
	bool SweepGround(float maxDistance, physx::PxSweepHit& outHit) const;

protected:
	void OnInitialize() override;
	void OnUpdate(float DeltaTime) override;

private:
	float m_Radius = 0.3f;
	float m_Height = 1.2f;

	bool bIsJumping = false;
	bool bIsOnGround = false;
	float m_VerticalVelocity = 0.f;
	float m_JumpForce = 8.0f;

	UTransformComponent* m_TransformComponent = nullptr;

	physx::PxQueryFilterData m_CollisionFilter;
	physx::PxController* m_CapsuleController = NULL;
};


#endif // K_CAPSULE_COMPONENT_HPP
