/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include "AbnormalEffectFactory.hpp"

class ResurrectionEffectFactory : public AbnormalEffectFactory
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
