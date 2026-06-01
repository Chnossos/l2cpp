/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include "../../constants/DamageElementType.hpp"
#include "EffectFactory.hpp"

class DamageEffectFactory : public EffectFactory
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

