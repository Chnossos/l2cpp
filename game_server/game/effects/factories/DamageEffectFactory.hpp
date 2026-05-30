/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include "../../constants/DamageElementType.hpp"
#include "AbnormalEffectFactory.hpp"

class DamageEffectFactory : public AbnormalEffectFactory
{
public:
    DamageEffectFactory(
        SkillTemplate     const & skillTemplate
      , EffectTargetType          targetType
      , SkillTargetNature         targetNature
      , DamageElementType         elementType
      , u32                       power
      , ClockDuration             totalDuration          = ClockDuration::zero()
      , ClockDuration             tickDuration           = ClockDuration::zero()
      , ClockDuration             initialTriggerDuration = ClockDuration::zero()
    );

public:
    void apply(Actor & source, Actor & target) override;

private:
    DamageElementType _elementType;
    u32               _power;
};

