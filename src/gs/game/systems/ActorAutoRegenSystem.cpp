/// @author    Chnossos
/// @date      Created on 2026-04-12

#include "ActorAutoRegenSystem.hpp"

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/components/ActorAutoRegen.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/network/packets/server/status/StatsUpdatePacket.hpp>

static constexpr auto gRegenTick = 3s;

/// @returns @c true if rounded-down stat has changed value, else @c false
static bool addRegenTicks(StatValue & stat, StatValue const regenPerSecond, size_t const ticks, StatValue const statMax)
{
    auto const tmp = stat;
    stat = std::min(stat + regenPerSecond * static_cast<double>(ticks), statMax);
    return stat != tmp;
}

void ActorAutoRegenSystem::updateImpl(ClockDuration const elapsed, Actor & actor)
{
    using enum StatId;

    auto const autoRegen = actor.component<ActorAutoRegen>();
    if (autoRegen && (autoRegen->elapsedSinceLastUpdate += elapsed) >= gRegenTick)
    {
        // A lot of time could have elapsed since last update, account for that
        size_t const ticks = std::chrono::floor<std::chrono::seconds>(autoRegen->elapsedSinceLastUpdate).count();

        autoRegen->elapsedSinceLastUpdate %= gRegenTick;

        auto & stats = *actor.component<Stats>();
        addRegenTicks(stats[CurHp], stats[HpRegen], ticks, stats[MaxHp]);
        addRegenTicks(stats[CurMp], stats[MpRegen], ticks, stats[MaxMp]);
        addRegenTicks(stats[CurCp], stats[CpRegen], ticks, stats[MaxCp]);
    }
}
