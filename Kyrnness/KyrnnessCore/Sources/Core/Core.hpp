/*********************************************************************
 *   File: Core.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef KYRNNES_CORE_RUNTIME_CORE_HPP
#define KYRNNES_CORE_RUNTIME_CORE_HPP

#include <memory>
#include <chrono>
#include <string>
#include <iostream>
#include <entt/entt.hpp>
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"

#include "Class.hpp"
#include "Core/TypesDefinition.hpp"
#include "Core/Math.hpp"
#include "Core/File.hpp"
#include "Core/Guid.hpp"
#include "Core/EventFunction.hpp"
#include "Core/Formatter.hpp"
#include "Core/Vector.hpp"
#include "Core/Quaternion.hpp"
#include "Core/Matrix.hpp"
#include "Core/Logger.hpp"

#define ThrowRuntimeError(A) FException::RuntimeError(A)
#define ThrowInvalidArgument(A) FException::InvalidArgument(A)

#define	KYRNNES_FAIL    0,
#define	KYRNNES_SUCCESS 1

struct FKyrnnessResult
{
	bool bSuccess = true;
	std::string Message;
};

class FTime
{
public:
	us::types::FCommonTypes::Time Time;

	FTime() = default;
	static FTime Now()
	{
		return FTime::FTime(us::types::FCommonTypes::Clock::now());
	}

private:
	FTime(us::types::FCommonTypes::Time newTime)
	{
		Time = newTime;
	}

};

class FClock
{
public:
	static FTime Now()
	{
		return FTime::Now();
	}
	static float Duration(const FTime& newTime, const FTime& currentTime)
	{
		return std::chrono::duration<float, us::types::FCommonTypes::Seconds::period>(newTime.Time - currentTime.Time).count();
	}
};

inline glm::vec3 ToVec3(const physx::PxExtendedVec3& pvec3)
{
	return glm::vec3(static_cast<float>(pvec3.x), static_cast<float>(pvec3.y), static_cast<float>(pvec3.z));
}

inline physx::PxExtendedVec3 ToPxExtendedVec3(const glm::vec3& gvec3)
{
	return physx::PxExtendedVec3(static_cast<float>(gvec3.x), static_cast<float>(gvec3.y), static_cast<float>(gvec3.z));
}

inline physx::PxVec3 ToPxVec3(const glm::vec3& gvec3)
{
	return physx::PxVec3(static_cast<float>(gvec3.x), static_cast<float>(gvec3.y), static_cast<float>(gvec3.z));
}

namespace World
{
	const FVector ZeroVector{ 0.f, 0.f, 0.f };
	const FVector UpVector{ 0.f, 1.f, 0.f };
	const FVector ForwardVector{ 1.f, 0.f, 0.f };
}

#endif // KYRNNES_CORE_RUNTIME_CORE_HPP