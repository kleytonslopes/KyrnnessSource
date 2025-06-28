/*********************************************************************
 *   File: Logger.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_CORE_LOGGER_HPP
#define K_CORE_LOGGER_HPP

#include <string>
#include <iostream>
#include <format>
#include <locale>
#include <fstream>

#include "Core/Formatter.hpp"

struct FLogger
{
private:
	static inline std::ofstream LogFile;

	static void WriteToFile(const std::string& level, const std::string& message)
	{
		if (!LogFile.is_open())
		{
			LogFile.open("Log.txt", std::ios::app); // append
		}
		LogFile << "[ " << level << " ]: " << message << std::endl;
	}

	template<typename... Args>
	static std::string FormatLog(const std::string& text, Args&&... args)
	{
		return FText::Format(text, std::forward<Args>(args)...);
	}

public:
	static void Open()
	{
		LogFile.open("Log.txt", std::ios::app);
	}

	static void Close()
	{
		if (LogFile.is_open())
			LogFile.close();
	}

	// ----------------- LOG LEVELS -----------------

	template<typename... Args>
	static __forceinline void Fatal(const std::string& text, Args&&... args)
	{
		auto msg = FormatLog(text, std::forward<Args>(args)...);
		std::cout << "\033[41;1;37m>> : " << msg << "\033[0m" << std::endl;
		WriteToFile("FATAL", msg);
	}
	static __forceinline void Fatal(const std::string& text)
	{
		std::cout << "\033[41;1;37m>> : " << text << "\033[0m" << std::endl;
		WriteToFile("FATAL", text);
	}

	template<typename... Args>
	static __forceinline void Error(const std::string& text, Args&&... args)
	{
		auto msg = FormatLog(text, std::forward<Args>(args)...);
		std::cout << "\033[5;31m>> : " << msg << "\033[0m" << std::endl;
		WriteToFile("ERROR", msg);
	}
	static __forceinline void Error(const std::string& text)
	{
		std::cout << "\033[5;31m>> : " << text << "\033[0m" << std::endl;
		WriteToFile("ERROR", text);
	}

	template<typename... Args>
	static __forceinline void Warning(const std::string& text, Args&&... args)
	{
		auto msg = FormatLog(text, std::forward<Args>(args)...);
		std::cout << "\033[5;33m>> : " << msg << "\033[0m" << std::endl;
		WriteToFile("WARNING", msg);
	}
	static __forceinline void Warning(const std::string& text)
	{
		std::cout << "\033[5;33m>> : " << text << "\033[0m" << std::endl;
		WriteToFile("WARNING", text);
	}

	template<typename... Args>
	static __forceinline void Information(const std::string& text, Args&&... args)
	{
		auto msg = FormatLog(text, std::forward<Args>(args)...);
		std::cout << "\033[5;36m>> : " << msg << "\033[0m" << std::endl;
		WriteToFile("INFO", msg);
	}
	static __forceinline void Information(const std::string& text)
	{
		std::cout << "\033[5;36m>> : " << text << "\033[0m" << std::endl;
		WriteToFile("INFO", text);
	}

	template<typename... Args>
	static __forceinline void Debug(const std::string& text, Args&&... args)
	{
		auto msg = FormatLog(text, std::forward<Args>(args)...);
		std::cout << "\033[1;33m>> : " << msg << "\033[0m" << std::endl;
		WriteToFile("DEBUG", msg);
	}
	static __forceinline void Debug(const std::string& text)
	{
		std::cout << "\033[1;33m>> : " << text << "\033[0m" << std::endl;
		WriteToFile("DEBUG", text);
	}

	template<typename... Args>
	static __forceinline void Trace(const std::string& text, Args&&... args)
	{
		auto msg = FormatLog(text, std::forward<Args>(args)...);
		std::cout << "\033[5;37m>> : " << msg << "\033[0m" << std::endl;
		WriteToFile("TRACE", msg);
	}
	static __forceinline void Trace(const std::string& text)
	{
		std::cout << "\033[5;37m>> : " << text << "\033[0m" << std::endl;
		WriteToFile("TRACE", text);
	}

	template<typename... Args>
	static __forceinline void Log(const std::string& text, Args&&... args)
	{
		auto msg = FormatLog(text, std::forward<Args>(args)...);
		std::cout << "\033[1;37m>> : " << msg << "\033[0m" << std::endl;
		WriteToFile("LOG", msg);
	}
	static __forceinline void Log(const std::string& text)
	{
		std::cout << "\033[1;37m>> : " << text << "\033[0m" << std::endl;
		WriteToFile("LOG", text);
	}

	template<typename... Args>
	static __forceinline void Success(const std::string& text, Args&&... args)
	{
		auto msg = FormatLog(text, std::forward<Args>(args)...);
		std::cout << "\033[1;32m>> : " << msg << "\033[0m" << std::endl;
		WriteToFile("SUCCESS", msg);
	}
	static __forceinline void Success(const std::string& text)
	{
		std::cout << "\033[1;32m>> : " << text << "\033[0m" << std::endl;
		WriteToFile("SUCCESS", text);
	}
};

struct FException
{
	template<typename... Args>
	static __forceinline void RuntimeError(const std::string& message, Args&&... args)
	{
		FLogger::Fatal(message, args);
		throw std::runtime_error(message);
	}
	static __forceinline void RuntimeError(const std::string& message)
	{
		FLogger::Fatal(message);
		throw std::runtime_error(message);
	}

	template<typename... Args>
	static __forceinline void InvalidArgument(const std::string& message, Args&&... args)
	{
		FLogger::Fatal(message, args);
		throw std::invalid_argument(message);
	}
	static __forceinline void InvalidArgument(const std::string& message)
	{
		FLogger::Fatal(message);
		throw std::invalid_argument(message);
	}
};

#endif// K_CORE_LOGGER_HPP
