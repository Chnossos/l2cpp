/// @author    Chnossos
/// @date      Created on 2026-03-26

// ReSharper disable file CppRedundantBooleanExpressionArgument

#pragma once

namespace Config
{
    constexpr inline bool enabled  = true;
    constexpr inline bool disabled = false;

#ifdef NDEBUG
    constexpr inline bool isDebugMode = false;
#else
    constexpr inline bool isDebugMode = true;
#endif

    constexpr inline bool hexdumpPackets = isDebugMode && disabled;
}
