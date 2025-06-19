#pragma once
#ifndef KYRNNESSS_PACKER_HPP
#define KYRNNESSS_PACKER_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdint>
#include <zlib.h>

struct FileEntry
{
	std::string relativePath;
	uint32_t offset;
	uint32_t uncompressedSize;
	uint32_t compressedSize;
	uint32_t crc32;
};

namespace fs = std::filesystem;

class UPacker
{

public:
#if defined(ENCRYPTION_XOR)
	static void PackDirectory(const fs::path& inputDir, const fs::path& outputFile) { PackDirectory_With_XOR(inputDir, outputFile); }
#else
	static void PackDirectory(const fs::path& inputDir, const fs::path& outputFile) { PackDirectory_NoEncryption(inputDir, outputFile); }
#endif

	static void XOREncrypt(std::vector<uint8_t>& data, uint8_t key);

private:
	static uint32_t crc32(const uint8_t* data, size_t length);

	static void PackDirectory_NoEncryption(const fs::path& inputDir, const fs::path& outputFile);
	static void PackDirectory_With_XOR(const fs::path& inputDir, const fs::path& outputFile);
};

#endif // KYRNNESSS_PACKER_HPP

