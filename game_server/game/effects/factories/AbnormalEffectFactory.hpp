/// @author    Chnossos
/// @date      Created on 2026-04-16

#pragma once

// Project includes
#include "../../../Typedefs.hpp"
#include "../../constants/AbnormalEffectType.hpp"
#include "../../constants/EffectTargetType.hpp"
#include "../../constants/SkillTargetNature.hpp"

class Actor;
class SkillTemplate;

class AbnormalEffectFactory
{
public:
    AbnormalEffectFactory(
        AbnormalEffectType         type
      , SkillTemplate      const & skillTemplate
      , EffectTargetType           targetType
      , SkillTargetNature          targetNature
      , ClockDuration              totalDuration          = ClockDuration::zero()
      , ClockDuration              tickDuration           = ClockDuration::zero()
      , ClockDuration              initialTriggerDuration = ClockDuration::zero()
    );
    virtual ~AbnormalEffectFactory() = default;

public:
    auto type()         const -> AbnormalEffectType { return _type;         }
    auto targetType()   const -> EffectTargetType   { return _targetType;   }
    auto targetNature() const -> SkillTargetNature  { return _targetNature; }

public:
    virtual void apply(Actor & source, Actor & target) = 0;

protected:
    SkillTemplate const & _skillTemplate;
    ClockDuration         _totalDuration;
    ClockDuration         _tickDuration;
    ClockDuration         _initialTriggerDuration;

private:
    AbnormalEffectType _type;
    EffectTargetType   _targetType;
    SkillTargetNature  _targetNature;
};
