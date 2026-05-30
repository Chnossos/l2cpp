/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include "AbnormalEffectFactory.hpp"

class HealEffectFactory : public AbnormalEffectFactory
{
public:
    HealEffectFactory(
        SkillTemplate     const & skillTemplate
      , EffectTargetType          targetType
      , SkillTargetNature         targetNature
      , u32                       power
      , ClockDuration             totalDuration          = ClockDuration::zero()
      , ClockDuration             tickDuration           = ClockDuration::zero()
      , ClockDuration             initialTriggerDuration = ClockDuration::zero()
    );

public:
    void apply(Actor & source, Actor & target) override;

private:
    u32 _power;
};

