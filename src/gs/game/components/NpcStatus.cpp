/// @author    Chnossos
/// @date      Created on 2026-05-10

#include "NpcStatus.hpp"

// Project includes
#include <common/core/Exception.hpp>
#include <gs/game/directories/NpcDirectory.hpp>
#include <gs/game/gameplay/ExperienceTable.hpp>

NpcStatus::NpcStatus()
    : _level(1)
{}

auto NpcStatus::level() const -> u32
{
    return _level;
}

void NpcStatus::setLevel(u32 const level, double)
{
    L2CPP_B_ASSERT(ExperienceTable::minLevel() <= level && level <= NpcDirectory::maxLevel(),
                   "Invalid level '{}'", level);

    _level = level;
}
