/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "BuffEffect.hpp"

// Project includes
#include "../../network/packets/server/status/CharacterStatusUpdateBroadcastPacket.hpp"
#include "../../network/packets/server/status/CharacterStatusUpdatePacket.hpp"
#include "../../network/packets/server/status/NpcStatusUpdatePacket.hpp"
#include "../World.hpp"
#include "../actor/Character.hpp"
#include "../actor/Npc.hpp"
#include "../components/Stats.hpp"

#include <l2cpp/utils/Enum.hpp>

namespace
{
    bool isSpeedStat(StatId const statId)
    {
        using enum StatId;
        return l2cpp::Utils::Enum::isAnyOf(statId, MoveSpeedMultiplier, MoveSpeedBonus,
                                                   PAtkSpeedMultiplier, PAtkSpeedBonus);
    }
}

BuffEffect::BuffEffect(
    Actor               & source
  , Actor               & target
  , SkillUid      const   skillUid
  , ClockDuration const   duration
  , StatId        const   modifiedStat
  , StatValue     const   value
)
    : Effect{EffectType::Buff, source, target, skillUid, duration}
    , _modifiedStat{modifiedStat}
    , _value{value}
{}

void BuffEffect::onStarted()
{
    modifyStat(_value);
}

void BuffEffect::onFinished()
{
    modifyStat(-_value);
}

void BuffEffect::modifyStat(StatValue const newValue) const
{
    auto & stats = *target().component<Stats>();
    stats[_modifiedStat] += newValue;
    stats.compute(target());

    namespace SC   = Network::Packet::Server;

    if (target().type() == ActorType::Character)
    {
        auto const & c = static_cast<Character const &>(target());
        World::send(c, SC::CharacterStatusUpdatePacket{c});

        if (isSpeedStat(_modifiedStat))
            World::broadcastAround(c, SC::CharacterStatusUpdateBroadcastPacket{c});
    }
    else
    {
        auto const & npc = static_cast<Npc const &>(target());
        World::broadcastAround(npc, Network::Packet::Server::NpcStatusUpdatePacket(npc));
    }
}

