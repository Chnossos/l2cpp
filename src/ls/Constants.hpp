/// @author    Chnossos
/// @date      Created on 2026-04-11

#pragma once

// Project includes
#include <l2cpp/Typedefs.hpp>

// C++ includes
#include <string_view>

namespace Constants
{
    constexpr inline std::string_view host = "127.0.0.1";
    constexpr inline auto             port = 2106;

    constexpr inline u32  protocol        = 0xc621;
    constexpr inline byte blowfishToken[] = "_;5.]94-31==-%xT!^[$"; // trailing zero is included by sizeof()
    constexpr inline auto rsaExponent     = "65537";
    constexpr inline auto rsaKeyBitSize   = 1024;
}
