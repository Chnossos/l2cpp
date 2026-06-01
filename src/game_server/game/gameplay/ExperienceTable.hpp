/// @author    Chnossos
/// @date      Created on 2026-04-28

#pragma once

// Project includes
#include "../../Typedefs.hpp"

class ExperienceTable
{
    ExperienceTable() noexcept = default;

public:
    static auto level(u32 experience) -> u32;
    static auto floor(u32 level) -> u32;
    static auto ceil(u32 level) -> u32;
    static auto minLevel() -> u32;
    static auto maxLevel() -> u32;
};
