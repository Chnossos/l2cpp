/// @author    Chnossos
/// @date      Created on 2026-04-11

#pragma once

// Project includes
#include <common/Typedefs.hpp>

// C++ includes
#include <span>

namespace Checksum
{
    u64 calculate(std::span<byte const> buffer);

    bool verify(std::span<byte const> buffer);
}
