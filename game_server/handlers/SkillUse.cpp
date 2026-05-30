/// @author    Chnossos
/// @date      Created on 2026-02-27

// Project includes
#include "../game/actions/SkillAction.hpp"
#include "../game/actor/Character.hpp"
#include "../game/components/SkillDirectory.hpp"
#include "../network/packets/server/action/ActionFailedPacket.hpp"
#include "_Common.hpp"

#include <l2cpp/utils/Enum.hpp>

DEFINE_PACKET_HANDLER(SkillUse) try
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    u32 skillId, forceAttack;
    bool disallowMovement;
    reader >> skillId >> forceAttack >> disallowMovement;

    auto & c         = *player.currentCharacter();
    auto const skill = c.skills().skill(static_cast<SkillId>(skillId));

    bool canCast = true;
    auto const updateCanCast = [&canCast] (bool const condition) { if (canCast) canCast = condition; };

    // Player has learned this skill
    updateCanCast(skill.has_value());

    // Skill is activatable
    using l2cpp::Utils::Enum::isAnyOf;
    updateCanCast(isAnyOf(skill->tmplate().type(), SkillType::Active, SkillType::Toggle));

    // Caster is alive and ready to start (or queue) a new skill
    using enum ActorState;
    updateCanCast(c.isAlive() && isAnyOf(c.state, Idle, CombatIdle, Casting, Moving));

    // Skill doesn't need a target, or else target is valid at the time of the request
    updateCanCast(!skill->tmplate().needsTarget() || c.target());

    if (canCast)
        c.doNext<SkillAction>(skill->tmplate(), forceAttack);
    else
        player.connection().send(ActionFailedPacket{});
}
catch (...)
{
    player.connection().send(ActionFailedPacket{});
    throw;
}
