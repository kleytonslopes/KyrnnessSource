/*********************************************************************
 *   File: PhysicsDebug.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_PHYSICS_DEBUG_HPP
#define K_PHYSICS_DEBUG_HPP

#include "Core/Core.hpp"

class UPhysicsDebug
{
public:
	UPhysicsDebug();
	virtual ~UPhysicsDebug() = default;

	UPhysicsDebug(const UPhysicsDebug&) = delete;
	UPhysicsDebug& operator=(const UPhysicsDebug&) = delete;
	UPhysicsDebug(UPhysicsDebug&&) = delete;
	UPhysicsDebug& operator=(UPhysicsDebug&&) = delete;

	virtual void Initialize();
	virtual void Update(float deltaTime);
	virtual void Destroy();

	virtual void DrawCapsule(const glm::vec3& position, float radius, float height, const glm::vec4& color);

};

#endif // K_PHYSICS_DEBUG_HPP
