/*********************************************************************
 *   File: File.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_FILE_HPP
#define K_FILE_HPP

#include <string>

class FFile
{
public:
	explicit FFile() { }
	virtual ~FFile() { }

	static std::string ReadFile(const std::string& filePath);
};

#endif // !K_FILE_HPP

