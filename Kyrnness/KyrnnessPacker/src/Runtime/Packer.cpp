#include "pch.hpp"
#include "Runtime/Packer.hpp"

void UPacker::PackDirectory(const fs::path& inputDir, const fs::path& outputFile)
{
    std::vector<FileEntry> entries;
    std::vector<std::vector<uint8_t>> fileDataBlocks;
    uint32_t currentOffset = 0;

    for (auto& p : fs::recursive_directory_iterator(inputDir))
    {
        std::cout << "Empacotando : " << p << "\n";
        if (p.is_regular_file())
        {
            fs::path relativePath = fs::relative(p.path(), inputDir);
            std::string pathStr = relativePath.generic_string();

            std::cout << "Empacotando Relative Path: " << relativePath << "\n";
            std::cout << "Empacotando Relative Path: " << pathStr << "\n";

            std::ifstream inFile(p.path(), std::ios::binary);
            std::vector<uint8_t> data((std::istreambuf_iterator<char>(inFile)),
                std::istreambuf_iterator<char>());

            std::vector<uint8_t> compressedData;
            uLongf compressedSize = compressBound(data.size());
            compressedData.resize(compressedSize);

            int result = compress2(compressedData.data(), &compressedSize, data.data(), data.size(), Z_BEST_COMPRESSION);
            if (result != Z_OK)
            {
                std::cerr << "Erro ao comprimir: " << p.path() << "\n";
                continue;
            }

            compressedData.resize(compressedSize);

            FileEntry entry;
            entry.relativePath = relativePath.generic_string();
            entry.uncompressedSize = static_cast<uint32_t>(data.size());
            entry.compressedSize = static_cast<uint32_t>(compressedData.size());

            // Calculo do CRC32
            entry.crc32 = ::crc32(0L, Z_NULL, 0);
            entry.crc32 = ::crc32(entry.crc32, data.data(), data.size());

            entries.push_back(entry);
            fileDataBlocks.push_back(std::move(compressedData));
        }
    }

    // Calcula offsets
    uint32_t headerSize = 4 + 4 + 4 + 4;  // Magic + Version + NumFiles + DataOffset
    uint32_t directorySize = 0;

    for (const auto& e : entries)
    {
        directorySize += sizeof(uint32_t);                        // Path size
        directorySize += static_cast<uint32_t>(e.relativePath.size());  // Path data (sem null terminator)
        directorySize += sizeof(uint32_t) * 4;                    // offset, uncompressedSize, compressedSize, crc32
    }

    currentOffset = headerSize + directorySize;

    for (auto& e : entries)
    {
        e.offset = currentOffset;
        currentOffset += e.compressedSize;
    }

    std::ofstream outFile(outputFile, std::ios::binary);

    // Header
    outFile.write("KGPK", 4);
    uint32_t version = 1;
    uint32_t numFiles = static_cast<uint32_t>(entries.size());
    uint32_t dataOffset = headerSize + directorySize;
    outFile.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));
    outFile.write(reinterpret_cast<char*>(&numFiles), sizeof(uint32_t));
    outFile.write(reinterpret_cast<char*>(&dataOffset), sizeof(uint32_t));

    // Directory
    for (const auto& e : entries)
    {
        std::cout << "Gravando Path: \"" << e.relativePath << "\"\n";

        uint32_t pathSize = static_cast<uint32_t>(e.relativePath.size());
        outFile.write(reinterpret_cast<const char*>(&pathSize), sizeof(uint32_t));
        outFile.write(e.relativePath.data(), pathSize);

        outFile.write(reinterpret_cast<const char*>(&e.offset), sizeof(uint32_t));
        outFile.write(reinterpret_cast<const char*>(&e.uncompressedSize), sizeof(uint32_t));
        outFile.write(reinterpret_cast<const char*>(&e.compressedSize), sizeof(uint32_t));
        outFile.write(reinterpret_cast<const char*>(&e.crc32), sizeof(uint32_t));
    }

    // Data blocks
    for (const auto& data : fileDataBlocks)
    {
        outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
    }

    outFile.close();

    std::cout << "Empacotamento concluído: " << outputFile << "\n";
}

uint32_t UPacker::crc32(const uint8_t* data, size_t length)
{
    static uint32_t table[256];
    static bool tableInitialized = false;

    if (!tableInitialized)
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            uint32_t crc = i;
            for (uint32_t j = 0; j < 8; j++)
                crc = (crc >> 1) ^ (0xEDB88320 & -(int)(crc & 1));
            table[i] = crc;
        }
        tableInitialized = true;
    }

    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++)
        crc = (crc >> 1) ^ table[(crc ^ data[i]) & 0xFF];
    return ~crc;
}
