#pragma once
#ifndef KYRNNESS_COLLISION_COMPONENT_HPP
#define KYRNNESS_COLLISION_COMPONENT_HPP

#include "Components/Component.hpp"

class FCollisionComponent : public UComponent
{
public:
	FCollisionComponent() = default;

	FCollisionComponent(const FCollisionComponent&) = delete;
	FCollisionComponent& operator=(const FCollisionComponent&) = delete;
	FCollisionComponent(FCollisionComponent&&) = delete;
	FCollisionComponent& operator=(FCollisionComponent&&) = delete;

	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual nlohmann::json GetJsonData() override;

	void SetMass(float mass);
	void SetBoundBox(const glm::vec3& boundBox);

protected:
	float m_Mass = 100.0f;

	glm::vec3 m_BoundBox{ 1.f, 1.f, 1.f };
	glm::vec3 m_InitialInertia{ 0.f, 0.f, 0.f };

	physx::PxRigidStatic* m_BodyStatic = nullptr;
	physx::PxRigidDynamic* m_BodyDynamic = nullptr;
};

#endif // KYRNNESS_COLLISION_COMPONENT_HPP
