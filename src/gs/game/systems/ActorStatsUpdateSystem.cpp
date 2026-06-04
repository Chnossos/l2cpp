/// @author    Chnossos
/// @date      Created on 2026-05-04

#include "ActorStatsUpdateSystem.hpp"

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/actor/Npc.hpp>
#include <gs/game/components/CharacterStatus.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/network/packets/server/action/SocialActionPerformPacket.hpp>
#include <gs/network/packets/server/status/CharacterStatusUpdatePacket.hpp>
#include <gs/network/packets/server/status/CharacterStatusUpdateBroadcastPacket.hpp>
#include <gs/network/packets/server/status/NpcStatusUpdatePacket.hpp>
#include <gs/network/packets/server/status/StatsUpdatePacket.hpp>

// C++ includes
#include <unordered_set>

namespace SC = Network::Packets::Server;

void ActorStatsUpdateSystem::updateImpl(ClockDuration, Actor & actor)
{
    static std::unordered_map<GameObjectId, Stats>           statSnapshots;
    static std::unordered_map<GameObjectId, CharacterStatus> statusSnapshots;

    if (auto const it = statSnapshots.find(actor.id()); it != statSnapshots.end())
    {
        if (actor.type() == ActorType::Character)
        {
            SC::StatsUpdatePacket privatePacket(actor), publicPacket(actor);

            bool const visualSpeedModified = updateCharacterStats(
                privatePacket, publicPacket, static_cast<Character &>(actor), it->second);

            updateCharacterStatus(privatePacket, publicPacket,
                                  static_cast<Character &>(actor), statusSnapshots.at(actor.id()));

            if (visualSpeedModified)
            {
                auto const & c = static_cast<Character const &>(actor);
                World::send(actor, SC::CharacterStatusUpdatePacket{c});
                World::broadcastAround(actor, SC::CharacterStatusUpdateBroadcastPacket{c});
            }
            else
            {
                if (!privatePacket.empty())
                    World::send(actor, std::move(privatePacket));

                if (!publicPacket.empty())
                    World::broadcastAround(actor, std::move(publicPacket));
            }
        }
        else
            updateNpcStats(static_cast<Npc &>(actor), it->second);
    }

    // Take a snapshot for the next update
    statSnapshots.insert_or_assign(actor.id(), actor.stats());
    if (actor.type() == ActorType::Character)
        statusSnapshots.insert_or_assign(actor.id(), static_cast<Character const &>(actor).status());
}

bool ActorStatsUpdateSystem::updateCharacterStats(SC::StatsUpdatePacket & privatePacket,
                                                  SC::StatsUpdatePacket & /*publicPacket*/,
                                                  Character & c, Stats const & oldStats) const
{
    bool visualSpeedModified = false;

    auto const & newStats = c.stats();
    for (size_t i = 0; i < std::to_underlying(StatId::Count); ++i)
    {
        if (oldStats[i] != newStats[i])
        {
            auto const statId = static_cast<StatId>(i);
            if (isVisualSpeedStat(statId))
            {
                visualSpeedModified = true;
                break; // Don't bother, we'll send a full update
            }
            privatePacket.addStat(statId, newStats[i]);
        }
    }

    if (newStats[StatId::CurHp] == 0 && oldStats[StatId::CurHp] > 0)
        c.die();

    return visualSpeedModified;
}

void ActorStatsUpdateSystem::updateCharacterStatus(SC::StatsUpdatePacket & privatePacket,
                                                   SC::StatsUpdatePacket & /*publicPacket*/,
                                                   Character & c, CharacterStatus const & oldStatus) const
{
    auto const & newStatus = c.status();

    if (newStatus.xp() != oldStatus.xp())
        privatePacket.addStat(Stat::Xp, newStatus.xp());

    if (newStatus.sp() != oldStatus.sp())
        privatePacket.addStat(Stat::Sp, newStatus.sp());

    if (newStatus.level() != oldStatus.level())
    {
        privatePacket.addStat(Stat::Level, newStatus.level());
        if (newStatus.level() > oldStatus.level())
            World::broadcastAround(c, SC::SocialActionPerformPacket{c, SocialActionId::LevelUpAnimation}, true);
    }
}

void ActorStatsUpdateSystem::updateNpcStats(Npc & npc, Stats const & oldStats) const
{
    static std::unordered_set publicStats { StatId::CurHp, StatId::MaxHp };

    auto const & newStats = npc.stats();
    bool visualSpeedModified = false;

    SC::StatsUpdatePacket publicPacket(npc);
    for (size_t i = 0; i < std::to_underlying(StatId::Count); ++i)
    {
        if (oldStats[i] != newStats[i])
        {
            auto const statId = static_cast<StatId>(i);

            visualSpeedModified |= isVisualSpeedStat(statId);

            if (publicStats.contains(statId))
                publicPacket.addStat(statId, newStats[i]);
        }
    }

    if (visualSpeedModified)
        World::broadcastAround(npc, SC::NpcStatusUpdatePacket{npc});

    if (!publicPacket.empty())
        World::broadcastToSubscribers(npc, std::move(publicPacket));

    if (newStats[StatId::CurHp] == 0 && oldStats[StatId::CurHp] > 0)
        npc.die();
}
