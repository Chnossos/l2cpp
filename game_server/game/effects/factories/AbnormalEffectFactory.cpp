/// @author    Chnossos
/// @date      Created on 2026-04-16

#include "AbnormalEffectFactory.hpp"

// Project includes
#include "../../actor/Actor.hpp"

AbnormalEffectFactory::AbnormalEffectFactory(
    AbnormalEffectType const   type
  , SkillTemplate      const & skillTemplate
  , EffectTargetType   const   targetType
  , SkillTargetNature  const   targetNature
  , ClockDuration      const   totalDuration
  , ClockDuration      const   tickDuration
  , ClockDuration      const   initialTriggerDuration
)
    : _skillTemplate{skillTemplate}
    , _totalDuration{totalDuration}
    , _tickDuration{tickDuration}
    , _initialTriggerDuration{initialTriggerDuration}
    , _type{type}
    , _targetType{targetType}
    , _targetNature{targetNature}
{}
