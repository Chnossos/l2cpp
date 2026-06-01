/// @author    Chnossos
/// @date      Created on 2026-04-22

#pragma once

// Project includes
#include "../game/constants/SkillTargetNature.hpp"

class Actor;

namespace Utils::Target
{
    bool isValidTarget(Actor const & emitter, SkillTargetNature, Actor const & target, bool forceAttack,
                       bool ignoreDeathStatus = false);
}
