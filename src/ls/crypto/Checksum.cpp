/// @author    Chnossos
/// @date      Created on 2026-04-11

#include "Checksum.hpp"

// Project includes
#include <common/core/Exception.hpp>

u64 Checksum::calculate(std::span<byte const> const buffer)
{
    L2CPP_B_ASSERT(buffer.size() % sizeof(u32) == 0, "Cannot calculate checksum: buf size not aligned to sizeof(u32)");

    auto       start = reinterpret_cast<u32 const *>(buffer.data());
    auto const end   = reinterpret_cast<u32 const *>(buffer.data() + buffer.size());

    u32 checksum = 0;
    while (start < end)
        checksum ^= *start++;

    return checksum; // pad to 8 bytes, required by Blowfish
}

bool Checksum::verify(std::span<byte const> const buffer)
{
    L2CPP_B_ASSERT(buffer.size() % sizeof(u64) == 0, "Cannot verify checksum: buf size not aligned to sizeof(u64)");
    auto const payload = buffer.subspan(0, buffer.size() - sizeof(u64));
    return calculate(payload) == *reinterpret_cast<u64 const *>(payload.data() + payload.size());
}
