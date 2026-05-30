/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "ToggleBuffEffectFactory.hpp"

ToggleBuffEffectFactory::ToggleBuffEffectFactory(SkillTemplate const & skillTemplate,
                                                 StatId const modifiedStat, StatValue const value)
        : BuffEffectFactory{skillTemplate, EffectTargetType::Self, SkillTargetNature::Self, -1s, modifiedStat, value}
{}

