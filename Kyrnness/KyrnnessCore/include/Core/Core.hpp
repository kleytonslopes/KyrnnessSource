#pragma once

#ifndef KYRNNES_CORE_RUNTIME_CORE_HPP
#define KYRNNES_CORE_RUNTIME_CORE_HPP

#include <memory>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <entt/entt.hpp>
#include <unordered_map>
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"

#include "Class.hpp"
#include "Core/Math.hpp"
#include "Core/File.hpp"
#include "Core/Guid.hpp"
#include "Core/EventFunction.hpp"
#include "Core/Formatter.hpp"
#include "Core/Vector.hpp"
#include "Core/Quaternion.hpp"
#include "Core/Matrix.hpp"
#include "Core/Logger.hpp"

namespace us::types
{
	struct FCommonTypes
	{
		using UUtint8 = unsigned char;
		using UUtint16 = unsigned short int;
		using UUtint32 = unsigned int;
		using UUtint64 = unsigned long long;

		using StInt8 = signed char;
		using StInt16 = signed short int;
		using StInt32 = signed int;
		using StInt64 = signed long long;

		using Clock = std::chrono::high_resolution_clock;
		using Time = std::chrono::high_resolution_clock::time_point;
		using Seconds = std::chrono::seconds;
	};
}

/** @brief Integer unsigned range 0 to 255 */
using uint8 = us::types::FCommonTypes::UUtint8;
/* @brief Integer unsigned range 0 to 65535 */
using uint16 = us::types::FCommonTypes::UUtint16;
/* @brief Integer unsigned range 0 to 4294967295 */
using uint32 = us::types::FCommonTypes::UUtint32;
/* @brief Integer unsigned range 0 to 18446744073709551615 */
using uint64 = us::types::FCommonTypes::UUtint64;

/* @brief Integer signed range -128 to 127 */
using int8 = us::types::FCommonTypes::StInt8;
/* @brief Integer signed range -32768 to 32767 */
using int16 = us::types::FCommonTypes::StInt16;
/* @brief Integer signed range -2147483648 to 2147483647 */
using int32 = us::types::FCommonTypes::StInt32;
/* @brief Integer signed range -9223372036854775808 to 9223372036854775807 */
using int64 = us::types::FCommonTypes::StInt64;

//using Clock = std::chrono::high_resolution_clock;
//using Time = std::chrono::high_resolution_clock::time_point;
//using Seconds = std::chrono::seconds;

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