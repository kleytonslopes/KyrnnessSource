#pragma once
#ifndef KYRNESS_TYPES_DEFINITION_HPP
#define KYRNESS_TYPES_DEFINITION_HPP

#include <unordered_map>
#include <functional>
#include <vector>
#include <thread>
#include <atomic>

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

template <typename T>
requires requires { typename std::function<T>; }
using TFunction = std::function<T>;

template<typename T>
using TAtomic = std::atomic<T>;

template<typename T>
using TVector = std::vector<T>;

template<typename K, typename T>
using TMap = std::unordered_map<K, T>;

using FThread = std::jthread;


#endif // KYRNESS_FREQUENCY_THREAD_HPP
