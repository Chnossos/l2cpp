/// @author    Chnossos
/// @date      Created on 2026-04-12

#include "ActorAutoRegenSystem.hpp"

// Project includes
#include "../../network/packets/server/status/StatsUpdatePacket.hpp"
#include "../World.hpp"
#include "../components/ActorAutoRegen.hpp"
#include "../components/Stats.hpp"

/// @returns @c true if rounded-down stat has changed value, else @c false
static bool addRegenTicks(StatValue & stat, StatValue const regenPerSecond, size_t const ticks, StatValue const statMax)
{
    auto const tmp = stat;
    stat = std::min(stat + regenPerSecond * ticks, statMax);
    return stat != tmp;
}

void ActorAutoRegenSystem::updateImpl(ClockDuration const elapsed, Actor & actor)
{
    using enum StatId;

    auto const autoRegen = actor.component<ActorAutoRegen>();
    if (autoRegen && (autoRegen->elapsedSinceLastUpdate += elapsed) >= 3s)
    {
        // A lot of time could have elapsed since last update, account for that
        size_t const ticks = std::chrono::floor<std::chrono::seconds>(autoRegen->elapsedSinceLastUpdate).count();

        autoRegen->elapsedSinceLastUpdate %= 3s;

        auto & stats = *actor.component<Stats>();
        std::unordered_map<Stat, StatValue> updates;

        if (addRegenTicks(stats[CurHp], stats[HpRegen], ticks, stats[MaxHp]))
            updates.try_emplace(Stat::CurHp, stats[CurHp]);

        if (addRegenTicks(stats[CurMp], stats[MpRegen], ticks, stats[MaxMp]))
            updates.try_emplace(Stat::CurMp, stats[CurMp]);

        if (addRegenTicks(stats[CurCp], stats[CpRegen], ticks, stats[MaxCp]))
            updates.try_emplace(Stat::CurCp, stats[CurCp]);

        if (!updates.empty())
        {
            Network::Packets::Server::StatsUpdatePacket p(actor);

            for (auto const [stat, value] : updates)
                p.addStat(stat, value);

            World::broadcastToSubscribers(actor, std::move(p), true);
        }
    }
}
