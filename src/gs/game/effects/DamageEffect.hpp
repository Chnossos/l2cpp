/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include <gs/game/constants/DamageElementType.hpp>
#include <gs/game/effects/Effect.hpp>

class DamageEffect : public Effect
{
public:
    DamageEffect(
        Actor             & source
      , Actor             & target
      , SkillUid            skillUid
      , DamageElementType   type
      , StatValue           damage
      , ClockDuration       effectDuration         = ClockDuration::zero()
      , ClockDuration       tickDuration           = ClockDuration::zero()
      , ClockDuration       initialTriggerDuration = ClockDuration::zero()
    );

private:
    void onTick() override;

private:
    DamageElementType _elementType;
    StatValue         _damage;
};

