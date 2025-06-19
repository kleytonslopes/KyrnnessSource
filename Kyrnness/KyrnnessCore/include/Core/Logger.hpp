#pragma once
#ifndef KYRNESS_CORE_LOGGER_HPP
#define KYRNESS_CORE_LOGGER_HPP

#include <string>
#include <iostream>
#include "Core/Formatter.hpp"

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

#ifdef DEBUG

#define LOG(A,B) krn::core::FLogger::A(B)
#define LOG_FATAL(A) krn::core::FLogger::Fatal(A)

#else

#define LOG(A,B)
#define LOG_FATAL(A)

#endif // DEBUG

#endif// KYRNESS_CORE_LOGGER_HPP
