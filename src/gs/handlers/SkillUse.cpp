/// @author    Chnossos
/// @date      Created on 2026-02-27

// Project includes
#include <common/utils/Enum.hpp>
#include <gs/game/actions/SkillAction.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/SkillDirectory.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/action/ActionFailedPacket.hpp>

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
    using Utils::Enum::isAnyOf;
    updateCanCast(isAnyOf(skill->tmplate().operatingType(), SkillOperatingType::Active, SkillOperatingType::Toggle));

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
