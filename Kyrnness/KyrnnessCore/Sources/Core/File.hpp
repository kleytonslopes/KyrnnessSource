#pragma once
#ifndef KYRNNESS_FILE_HPP
#define KYRNNESS_FILE_HPP

#include <string>

class FFile
{
public:
	explicit FFile() { }
	virtual ~FFile() { }

	static std::string ReadFile(const std::string& filePath);
};

#endif // !KYRNNESS_FILE_HPP

