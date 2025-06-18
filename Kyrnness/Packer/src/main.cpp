#include "pch.hpp"
#include "Runtime/Packer.hpp"

//namespace fs = std::filesystem;
//
//struct FileEntry
//{
//    std::string relativePath;
//    uint32_t offset;
//    uint32_t uncompressedSize;
//    uint32_t compressedSize;
//    uint32_t crc32;
//};
//
//uint32_t crc32(const uint8_t* data, size_t length)
//{
//    static uint32_t table[256];
//    static bool tableInitialized = false;
//
//    if (!tableInitialized)
//    {
//        for (uint32_t i = 0; i < 256; i++)
//        {
//            uint32_t crc = i;
//            for (uint32_t j = 0; j < 8; j++)
//                crc = (crc >> 1) ^ (0xEDB88320 & -(int)(crc & 1));
//            table[i] = crc;
//        }
//        tableInitialized = true;
//    }
//
//    uint32_t crc = 0xFFFFFFFF;
//    for (size_t i = 0; i < length; i++)
//        crc = (crc >> 1) ^ table[(crc ^ data[i]) & 0xFF];
//    return ~crc;
//}
//
//void PackDirectory(const fs::path& inputDir, const fs::path& outputFile)
//{
//    std::vector<FileEntry> entries;
//    std::vector<std::vector<uint8_t>> fileDataBlocks;
//    uint32_t currentOffset = 0;
//
//    for (auto& p : fs::recursive_directory_iterator(inputDir))
//    {
//        std::cout << "Empacotando : " << p << "\n";
//        if (p.is_regular_file())
//        {
//            fs::path relativePath = fs::relative(p.path(), inputDir);
//
//            std::ifstream inFile(p.path(), std::ios::binary);
//            std::vector<uint8_t> data((std::istreambuf_iterator<char>(inFile)),
//                std::istreambuf_iterator<char>());
//
//            std::vector<uint8_t> compressedData;
//            uLongf compressedSize = compressBound(data.size());
//            compressedData.resize(compressedSize);
//
//            int result = compress2(compressedData.data(), &compressedSize, data.data(), data.size(), Z_BEST_COMPRESSION);
//            if (result != Z_OK)
//            {
//                std::cerr << "Erro ao comprimir: " << p.path() << "\n";
//                continue;
//            }
//
//            compressedData.resize(compressedSize);
//
//            FileEntry entry;
//            entry.relativePath = relativePath.generic_string();
//            entry.uncompressedSize = static_cast<uint32_t>(data.size());
//            entry.compressedSize = static_cast<uint32_t>(compressedData.size());
//
//            // Calculo do CRC32
//            entry.crc32 = ::crc32(0L, Z_NULL, 0);
//            entry.crc32 = ::crc32(entry.crc32, data.data(), data.size());
//
//            entries.push_back(entry);
//            fileDataBlocks.push_back(std::move(compressedData));
//        }
//    }
//
//    // Calcula offsets
//    uint32_t headerSize = 4 + 4 + 4 + 4;  // Magic + Version + NumFiles + DataOffset
//    uint32_t directorySize = 0;
//
//    for (const auto& e : entries)
//    {
//        directorySize += sizeof(uint32_t);                        // Path size
//        directorySize += static_cast<uint32_t>(e.relativePath.size());  // Path data (sem null terminator)
//        directorySize += sizeof(uint32_t) * 4;                    // offset, uncompressedSize, compressedSize, crc32
//    }
//
//    currentOffset = headerSize + directorySize;
//
//    for (auto& e : entries)
//    {
//        e.offset = currentOffset;
//        currentOffset += e.compressedSize;
//    }
//
//    std::ofstream outFile(outputFile, std::ios::binary);
//
//    // Header
//    outFile.write("KGPK", 4);
//    uint32_t version = 1;
//    uint32_t numFiles = static_cast<uint32_t>(entries.size());
//    uint32_t dataOffset = headerSize + directorySize;
//    outFile.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));
//    outFile.write(reinterpret_cast<char*>(&numFiles), sizeof(uint32_t));
//    outFile.write(reinterpret_cast<char*>(&dataOffset), sizeof(uint32_t));
//
//    // Directory
//    for (const auto& e : entries)
//    {
//        uint32_t pathSize = static_cast<uint32_t>(e.relativePath.size());
//        outFile.write(reinterpret_cast<const char*>(&pathSize), sizeof(uint32_t));
//        outFile.write(e.relativePath.data(), pathSize);
//
//        outFile.write(reinterpret_cast<const char*>(&e.offset), sizeof(uint32_t));
//        outFile.write(reinterpret_cast<const char*>(&e.uncompressedSize), sizeof(uint32_t));
//        outFile.write(reinterpret_cast<const char*>(&e.compressedSize), sizeof(uint32_t));
//        outFile.write(reinterpret_cast<const char*>(&e.crc32), sizeof(uint32_t));
//    }
//
//    // Data blocks
//    for (const auto& data : fileDataBlocks)
//    {
//        outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
//    }
//
//    outFile.close();
//
//    std::cout << "Empacotamento concluído: " << outputFile << "\n";
//}

int main()
{
    fs::path inputDir = "../../Kyrnness/GameContent";
    fs::path outputFile = "../../Kyrnness/Build/KyrnnessGameData.gdata";

    UPacker::PackDirectory(inputDir, outputFile);

    return 0;
}

//#include "pch.hpp"
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <filesystem>
//#include <cstdint>
//#include <cstring>
//#include <zlib.h>
//
//namespace fs = std::filesystem;
//
//struct FileEntry
//{
//    std::string Path;
//    uint64_t Offset;
//    uint32_t SizeUncompressed;
//    uint32_t SizeCompressed;
//    uint32_t Checksum;
//};
//
//uint32_t CalculateCRC32(const std::vector<uint8_t>& data)
//{
//    return crc32(0L, data.data(), data.size());
//}
//
//void WriteUInt32(std::ofstream& out, uint32_t value)
//{
//    out.write(reinterpret_cast<const char*>(&value), sizeof(uint32_t));
//}
//
//void WriteUInt64(std::ofstream& out, uint64_t value)
//{
//    out.write(reinterpret_cast<const char*>(&value), sizeof(uint64_t));
//}
//
//int main()
//{
//    std::string inputFolder = "GameContent";
//    std::string outputFile = "Content.gdata";
//
//    std::vector<FileEntry> entries;
//    std::vector<std::vector<uint8_t>> fileData;
//
//    uint64_t currentOffset = 0;
//
//    // Reservado pro header + index (será atualizado no final)
//    currentOffset = 1024; // Espaço arbitrário pra evitar sobrescrever index depois (ajustaremos no final)
//
//    for (const auto& file : fs::recursive_directory_iterator(inputFolder))
//    {
//        if (file.is_regular_file())
//        {
//            std::ifstream inFile(file.path(), std::ios::binary);
//            if (!inFile)
//            {
//                std::cerr << "Falha ao abrir: " << file.path() << std::endl;
//                continue;
//            }
//
//            // Lê o conteúdo
//            std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
//
//            FileEntry entry;
//            entry.Path = fs::relative(file.path(), inputFolder).generic_string();
//            entry.Offset = currentOffset;
//            entry.SizeUncompressed = static_cast<uint32_t>(buffer.size());
//            entry.SizeCompressed = entry.SizeUncompressed; // (sem compressão por enquanto)
//            entry.Checksum = CalculateCRC32(buffer);
//
//            entries.push_back(entry);
//            fileData.push_back(std::move(buffer));
//
//            currentOffset += entry.SizeCompressed;
//        }
//    }
//
//    std::ofstream out(outputFile, std::ios::binary);
//    if (!out)
//    {
//        std::cerr << "Erro ao criar o arquivo de saída!" << std::endl;
//        return -1;
//    }
//
//    // --- HEADER ---
//    out.write("KGDATA", 6);
//    WriteUInt32(out, 1); // Version
//    WriteUInt32(out, static_cast<uint32_t>(entries.size()));
//    WriteUInt64(out, 1024); // Offset fixo pro index (melhoramos depois se quiser)
//
//    // --- Placeholder até o offset 1024 ---
//    out.seekp(1024, std::ios::beg);
//
//    // --- INDEX ---
//    for (const auto& entry : entries)
//    {
//        WriteUInt32(out, static_cast<uint32_t>(entry.Path.size()));
//        out.write(entry.Path.data(), entry.Path.size());
//        WriteUInt64(out, entry.Offset);
//        WriteUInt32(out, entry.SizeUncompressed);
//        WriteUInt32(out, entry.SizeCompressed);
//        WriteUInt32(out, entry.Checksum);
//    }
//
//    // --- DATA BLOBS ---
//    for (size_t i = 0; i < entries.size(); ++i)
//    {
//        out.write(reinterpret_cast<const char*>(fileData[i].data()), fileData[i].size());
//    }
//
//    out.close();
//
//    std::cout << "Empacotamento finalizado: " << outputFile << std::endl;
//    return 0;
//}
