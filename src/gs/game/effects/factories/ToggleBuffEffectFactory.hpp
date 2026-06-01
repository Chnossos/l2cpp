/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include <gs/game/effects/factories/BuffEffectFactory.hpp>

struct ToggleBuffEffectFactory : public BuffEffectFactory
{
    ToggleBuffEffectFactory(SkillTemplate const & skillTemplate, StatId modifiedStat, StatValue value);
};

