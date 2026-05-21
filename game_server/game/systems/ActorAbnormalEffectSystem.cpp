/// @author    Chnossos
/// @date      Created on 2026-04-14

#include "ActorAbnormalEffectSystem.hpp"

// Project includes
#include "../../network/packets/server/chat/ChatSystemSayPacket.hpp"
#include "../World.hpp"

#include <l2cpp/utils/Enum.hpp>

/// Update the effect list if we remove any non-damage effect, or it's damage over time
static bool needToUpdateList(bool const currentValue, AbnormalEffect const & effect)
{
    using enum AbnormalEffectType;
    return currentValue                                             // short-circuit
        || l2cpp::Utils::Enum::isAnyOf(effect.type(), Buff, Debuff) // Non hp modifying effect
        || effect.duration() != ClockDuration::zero();              // is damage or heal over time (it has an icon)
}

void ActorAbnormalEffectSystem::updateImpl(ClockDuration const elapsed, Actor & actor)
{
    bool updateList = false;

    std::list<SkillUid> addedEffects, removedEffects;

    auto & effects = actor.abnormalEffects();
    for (auto it = effects.begin(); it != effects.end(); )
    {
        if ((*it)->elapsed() == ClockDuration::zero()) // Effect just got applied
        {
            addedEffects.emplace_back((*it)->skillUid());
            updateList = needToUpdateList(updateList, **it);
        }

        if ((*it)->update(elapsed))
        {
            removedEffects.emplace_back((*it)->skillUid());
            updateList = needToUpdateList(updateList, **it);
            it = effects.erase(it);
        }
        else
            ++it;
    }

    if (updateList)
    {
        addedEffects.unique();
        for (auto const skillUid : addedEffects)
        {
            Network::Packet::Server::ChatSystemSayPacket msg{SystemMessageId::YouCanFeel_1sEffect};
            msg.appendName(skillUid);
            World::send(actor, std::move(msg));
        }

        removedEffects.unique();
        for (auto const skillUid : removedEffects)
        {
            Network::Packet::Server::ChatSystemSayPacket msg{SystemMessageId::TheEffectOf_1_HasWornOff};
            msg.appendName(skillUid);
            World::send(actor, std::move(msg));
        }

        fire actor.onAbnormalEffectListChanged();
    }
}
