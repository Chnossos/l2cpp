/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include "EffectFactory.hpp"

class ResurrectionEffectFactory : public EffectFactory
{
public:
    ResurrectionEffectFactory(
        SkillTemplate     const & skillTemplate
      , EffectTargetType          targetType
      , SkillTargetNature         targetNature
    );

public:
    void apply(Actor & source, Actor & target) override;
};
