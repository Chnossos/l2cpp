/// @author    Chnossos
/// @date      Created on 2026-05-10

#pragma once

// Project includes
#include <gs/game/components/ActorStatus.hpp>

/// @note NPCs start at level 1 unless overriden.
class NpcStatus final : public ActorStatus
{
public:
    NpcStatus();

public:
    auto level() const -> u32 override;

public:
    /// @param level   Desired level for this NPC (between min and max level of course).
    /// @param percent Ignored.
    void setLevel(u32 level, double percent) override;
    using ActorStatus::setLevel;

private:
    u32 _level;
};
