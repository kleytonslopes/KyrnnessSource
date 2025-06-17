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

#include "Core/Math.hpp"
#include "Core/File.hpp"
#include "Core/Guid.hpp"
#include "Core/EventFunction.hpp"
#include "Core/Formatter.hpp"
#include "Core/Vector.hpp"
#include "Core/Quaternion.hpp"
#include "Core/Matrix.hpp"

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


namespace krn::core
{
	struct FLogger
	{
		static void Fatal(const std::string& text)
		{
			std::cout << "\033[41;1;37m>> : " << text << "\033[0m" << std::endl;
		}

		static void Error(const std::string& text)
		{
			std::cout << "\033[5;31m>> : " << text << "\033[0m" << std::endl;
		}

		static void Warning(const std::string& text)
		{
			std::cout << "\033[5;33m>> : " << text << "\033[0m" << std::endl;
		}

		static void Information(const std::string& text)
		{
			std::cout << "\033[5;36m>> : " << text << "\033[0m" << std::endl;
		}

		static void Debug(const std::string& text)
		{
			std::cout << "\033[1;33m>> : " << text << "\033[0m" << std::endl;
		}

		static void Trace(const std::string& text)
		{
			std::cout << "\033[5;37m>> : " << text << "\033[0m" << std::endl;
		}

		static void Log(const std::string& text)
		{
			std::cout << "\033[1;37m>> : " << text << "\033[0m" << std::endl;
		}

		static void Success(const std::string& text)
		{
			std::cout << "\033[1;32m>> : " << text << "\033[0m" << std::endl;
		}
	};

	struct FException
	{
		static void RuntimeError(const std::string& message)
		{
			FLogger::Fatal(message);
			throw std::runtime_error(message);
		}

		static void InvalidArgument(const std::string& message)
		{
			FLogger::Fatal(message);
			throw std::invalid_argument(message);
		}
	};
}

#define LOG(A,B) krn::core::FLogger::A(B)
#define LOG_FATAL(A) krn::core::FLogger::Fatal(A)

#define ThrowRuntimeError(A) krn::core::FException::RuntimeError(A)
#define ThrowInvalidArgument(A) krn::core::FException::InvalidArgument(A)

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

#endif // KYRNNES_CORE_RUNTIME_CORE_HPP