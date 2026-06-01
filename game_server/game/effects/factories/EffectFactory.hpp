/// @author    Chnossos
/// @date      Created on 2026-04-16

#pragma once

// Project includes
#include "../../../Typedefs.hpp"
#include "../../constants/EffectType.hpp"
#include "../../constants/EffectTargetType.hpp"
#include "../../constants/SkillTargetNature.hpp"

class Actor;
class SkillTemplate;

class EffectFactory
{
public:
    EffectFactory(
        EffectType                type
      , SkillTemplate     const & skillTemplate
      , EffectTargetType          targetType
      , SkillTargetNature         targetNature
      , ClockDuration             totalDuration          = ClockDuration::zero()
      , ClockDuration             tickDuration           = ClockDuration::zero()
      , ClockDuration             initialTriggerDuration = ClockDuration::zero()
    );
    virtual ~EffectFactory() = default;

public:
    auto type()                   const -> EffectType            { return _type;                   }
    auto skill()                  const -> SkillTemplate const & { return _skillTemplate;          }
    auto targetType()             const -> EffectTargetType      { return _targetType;             }
    auto targetNature()           const -> SkillTargetNature     { return _targetNature;           }
    auto totalDuration()          const -> ClockDuration         { return _totalDuration;          }
    auto tickDuration()           const -> ClockDuration         { return _tickDuration;           }
    auto initialTriggerDuration() const -> ClockDuration         { return _initialTriggerDuration; }

public:
    virtual void apply(Actor & source, Actor & target) = 0;

private:
    EffectType            _type;
    SkillTemplate const & _skillTemplate;
    EffectTargetType      _targetType;
    SkillTargetNature     _targetNature;
    ClockDuration         _totalDuration;
    ClockDuration         _tickDuration;
    ClockDuration         _initialTriggerDuration;
};
