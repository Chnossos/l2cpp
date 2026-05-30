/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include "Effect.hpp"

class HealEffect : public Effect
{
public:
    HealEffect(
        Actor         & source
      , Actor         & target
      , SkillUid        skillUid
      , StatValue       healAmount
      , ClockDuration   effectDuration         = ClockDuration::zero()
      , ClockDuration   tickDuration           = ClockDuration::zero()
      , ClockDuration   initialTriggerDuration = ClockDuration::zero()
    );

private:
    void onTick() override;

private:
    StatValue _healAmount;
};

