#include "pch.hpp"
#include "Core/File.hpp"

std::string FFile::ReadFile(const std::string& filePath)
{
	std::string fileContent;

	if (std::ifstream fileStream{filePath, std::ios::in})
	{
		fileContent.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	}

	return fileContent;
}
