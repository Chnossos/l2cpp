/// @author    Chnossos
/// @date      Created on 2026-05-10

#include "NpcStatus.hpp"

// Project includes
#include "../gameplay/ExperienceTable.hpp"

#include <common/core/Exception.hpp>

NpcStatus::NpcStatus()
    : _level(ExperienceTable::maxLevel())
{}

auto NpcStatus::level() const -> u32
{
    return _level;
}

void NpcStatus::setLevel(u32 const level, double)
{
    L2CPP_B_ASSERT(ExperienceTable::minLevel() <= level && level <= ExperienceTable::maxLevel(),
                   "Invalid level '{}'", level);

    _level = level;
}
