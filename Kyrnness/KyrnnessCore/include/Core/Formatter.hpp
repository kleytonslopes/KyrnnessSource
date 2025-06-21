/*********************************************************************
 *   File: Formatter.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: July 2023
 *
 * Copyright (c) 2023 Kyrnness. All rights reserved.
 *********************************************************************/
#pragma once

#ifndef KYRNNES_CORE_FORMATTER_HPP
#define	KYRNNES_CORE_FORMATTER_HPP

#include <string>
#include <regex>

namespace krnformat
{
	template<typename ... Args>
	std::string str_format(const std::string& format, Args ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
		if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
		auto size = static_cast<size_t>(size_s);
		std::unique_ptr<char[]> buf(new char[size]);
		std::snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1);
	}
}

struct FText
{
	template<typename ... Args>
	static std::string Format(const std::string& format, Args ... args)
	{
		return krnformat::str_format(format, args ...);
	}

	static std::string Replace(const std::string& text, const std::string& repl, const std::string& remov)
	{
		return std::regex_replace(text, std::regex(repl), remov);
	}

	static std::string Concat(std::string& left, const std::string& right)
	{
		return left.append(right);
	}
};

#endif // !KYRNNES_CORE_FORMATTER_HPP