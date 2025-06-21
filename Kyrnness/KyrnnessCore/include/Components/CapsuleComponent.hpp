#pragma once

#ifndef KYRNNESS_CAPSULE_COMPONENT_HPP
#define KYRNNESS_CAPSULE_COMPONENT_HPP

#include "Components/Component.hpp"

class FTransformComponent;

class FCapsuleComponent : public UComponent
{
public:
	FCapsuleComponent();

	virtual ~FCapsuleComponent() = default;

	FCapsuleComponent(const FCapsuleComponent&) = delete;
	FCapsuleComponent& operator=(const FCapsuleComponent&) = delete;
	FCapsuleComponent(FCapsuleComponent&&) = delete;
	FCapsuleComponent& operator=(FCapsuleComponent&&) = delete;

	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
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
private:
	float m_Radius = 0.3f;
	float m_Height = 1.2f;

	bool bIsJumping = false;
	bool bIsOnGround = false;
	float m_VerticalVelocity = 0.f;
	float m_JumpForce = 8.0f;

	FTransformComponent* m_TransformComponent = nullptr;

	physx::PxQueryFilterData m_CollisionFilter;
	physx::PxController* m_CapsuleController = NULL;
};


#endif // KYRNNESS_CAPSULE_COMPONENT_HPP
