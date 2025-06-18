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
    static void PackDirectory(const fs::path& inputDir, const fs::path& outputFile);

private:
    static uint32_t crc32(const uint8_t* data, size_t length);
};

#endif // KYRNNESSS_PACKER_HPP

