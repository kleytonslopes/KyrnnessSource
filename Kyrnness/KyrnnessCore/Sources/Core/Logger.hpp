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
#include "Core/Formatter.hpp"

struct FLogger
{
	template<typename... Args>
	static __forceinline void Fatal(const std::string& text, Args&&... args)
	{
		std::cout << "\033[41;1;37m>> : " << FText::Format(text, std::forward<Args>(args)...) << "\033[0m" << std::endl;
	}
	static __forceinline void Fatal(const std::string& text)
	{
		std::cout << "\033[41;1;37m>> : " << text << "\033[0m" << std::endl;
	}
	
	template<typename... Args>
	static __forceinline void Error(const std::string& text, Args&&... args)
	{
		std::cout << "\033[5;31m>> : " << FText::Format(text, std::forward<Args>(args)...) << "\033[0m" << std::endl;
	}
	static __forceinline void Error(const std::string& text)
	{
		std::cout << "\033[5;31m>> : " << text << "\033[0m" << std::endl;
	}

	template<typename... Args>
	static __forceinline void Warning(const std::string& text, Args&&... args)
	{
		std::cout << "\033[5;33m>> : " << FText::Format(text, std::forward<Args>(args)...) << "\033[0m" << std::endl;
	}
	static __forceinline void Warning(const std::string& text)
	{
		std::cout << "\033[5;33m>> : " << text << "\033[0m" << std::endl;
	}

	template<typename... Args>
	static __forceinline void Information(const std::string& text, Args&&... args)
	{
		std::cout << "\033[5;36m>> : " << FText::Format(text, std::forward<Args>(args)...) << "\033[0m" << std::endl;
	}
	static __forceinline void Information(const std::string& text)
	{
		std::cout << "\033[5;36m>> : " << text << "\033[0m" << std::endl;
	}

	template<typename... Args>
	static __forceinline void Debug(const std::string& text, Args&&... args)
	{
		std::cout << "\033[1;33m>> : " << FText::Format(text, std::forward<Args>(args)...) << "\033[0m" << std::endl;
	}
	static __forceinline void Debug(const std::string& text)
	{
		std::cout << "\033[1;33m>> : " << text << "\033[0m" << std::endl;
	}

	template<typename... Args>
	static __forceinline void Trace(const std::string& text, Args&&... args)
	{
		std::cout << "\033[5;37m>> : " << FText::Format(text, std::forward<Args>(args)...) << "\033[0m" << std::endl;
	}
	static __forceinline void Trace(const std::string& text)
	{
		std::cout << "\033[5;37m>> : " << text << "\033[0m" << std::endl;
	}

	template<typename... Args>
	static __forceinline void Log(const std::string& text, Args&&... args)
	{
		std::cout << "\033[1;37m>> : " << FText::Format(text, std::forward<Args>(args)...) << "\033[0m" << std::endl;
	}
	static __forceinline void Log(const std::string& text)
	{
		std::cout << "\033[1;37m>> : " << text << "\033[0m" << std::endl;
	}

	template<typename... Args>
	static __forceinline void Success(const std::string& text, Args&&... args)
	{
		std::cout << "\033[1;32m>> : " << FText::Format(text, std::forward<Args>(args)...) << "\033[0m" << std::endl;
	}
	static __forceinline void Success(const std::string& text)
	{
		std::cout << "\033[1;32m>> : " << text << "\033[0m" << std::endl;
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
