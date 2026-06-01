/// @author    Chnossos
/// @date      Created on 2026-04-22

#include "Target.hpp"

// Project includes
#include "../game/actor/Actor.hpp"
#include "../game/skill/SkillTemplate.hpp"

#include <l2cpp/utils/EnumMask.hpp>

bool Utils::Target::isValidTarget(Actor const & emitter, SkillTargetNature const targetNature,
                                  Actor const & target, bool const forceAttack, bool const ignoreDeathStatus)
{
    using enum SkillTargetNature;

    EnumMask const nature = targetNature;

    if (nature == None) [[unlikely]]
        return false;

    if (nature & Corpse)
        return target.isAlive() ? false : isValidTarget(emitter, nature & ~Corpse, target, forceAttack, true);

    if (!target.isAlive() && !ignoreDeathStatus)
        return false;

    if (target == emitter)
        return nature & Self || nature & Party || nature & Clan || nature & Alliance;

    if (nature & Party || nature & Clan || nature & Alliance)
        return false; // TODO

    switch (target.type())
    {
        case ActorType::Npc:
        case ActorType::Character:
            return (nature & Friendly || forceAttack) || nature & Character;

        case ActorType::Monster:
            return nature & Enemy || nature & Monster;
    }

    return false;
}
