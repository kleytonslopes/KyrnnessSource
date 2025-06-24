#pragma once

#ifndef KYRNNESS_GUID_HPP
#define KYRNNESS_GUID_HPP

#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <compare>

class FGuid
{
private:
    uint32_t A, B, C, D;

public:
    FGuid()
        : A(0), B(0), C(0), D(0) {}

    FGuid(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
        : A(a), B(b), C(c), D(d) {}

    static FGuid NewGuid()
    {
        static thread_local std::mt19937_64 rng{ std::random_device{}() };
        std::uniform_int_distribution<uint32_t> dist;

        return FGuid(dist(rng), dist(rng), dist(rng), dist(rng));
    }

    std::string ToString() const
    {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0')
            << std::setw(8) << A << "-"
            << std::setw(4) << ((B >> 16) & 0xFFFF) << "-"
            << std::setw(4) << (B & 0xFFFF) << "-"
            << std::setw(4) << ((C >> 16) & 0xFFFF) << "-"
            << std::setw(4) << (C & 0xFFFF)
            << std::setw(8) << D;
        return oss.str();
    }

    // Compara��es
    auto operator<=>(const FGuid&) const = default;
    bool operator==(const FGuid&) const = default;

    // Para TMap
    struct Hash
    {
        size_t operator()(const FGuid& guid) const noexcept
        {
            return std::hash<uint32_t>{}(guid.A) ^
                std::hash<uint32_t>{}(guid.B) ^
                std::hash<uint32_t>{}(guid.C) ^
                std::hash<uint32_t>{}(guid.D);
        }
    };
};

#endif // !KYRNNESS_GUID_HPP
