/// @author    Chnossos
/// @date      Created on 2026-04-16

#include "EffectListPacket.hpp"

// Project includes
#include "../../../../game/actor/Actor.hpp"
#include "../../../../game/skill/SkillUid.hpp"

using Network::Packet::Server::EffectListPacket;

EffectListPacket::EffectListPacket(Actor const & actor)
    : Packet(0x7f, "EffectList")
{
    auto const & effects = actor.effects();

    std::list<std::pair<SkillUid, ClockDuration>> uniqueSkillEffects;

    for (auto const & effect : effects)
    {
        if (effect->remainingDuration() != ClockDuration::zero())
            uniqueSkillEffects.emplace_back(std::make_pair(effect->skillUid(), effect->remainingDuration()));
    }

    uniqueSkillEffects.unique();

    *this
        << static_cast<u16>(uniqueSkillEffects.size())
    ;

    for (auto const [skillUid, remainingDuration] : uniqueSkillEffects)
    {
        *this
            << skillUid
            << std::chrono::floor<std::chrono::seconds>(remainingDuration)
        ;
    }
}
