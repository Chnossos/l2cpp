/// @author    Chnossos
/// @date      Created on 2026-04-08

#include "SkillSetTargetsPacket.hpp"

// Project includes
#include "../../../../game/actor/Actor.hpp"
#include "../../../../game/skill/Skill.hpp"

using Network::Packets::Server::SkillSetTargetsPacket;

SkillSetTargetsPacket::SkillSetTargetsPacket(Actor const & caster, SkillTemplate const & skill,
                                             std::span<Ref<Actor const> const> const targets)
    : Packet(0x76, "SkillSetTargets")
{
    *this
        << caster.id()
        << skill.uid()
        << static_cast<u32>(targets.size())
    ;

    for (Actor const & target : targets)
        *this << target.id();
}
