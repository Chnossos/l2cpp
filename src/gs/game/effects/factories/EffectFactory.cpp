/// @author    Chnossos
/// @date      Created on 2026-04-16

#include "EffectFactory.hpp"

// Project includes
#include <gs/game/actor/Actor.hpp>

EffectFactory::EffectFactory(
    EffectType        const   type
  , SkillTemplate     const & skillTemplate
  , EffectTargetType  const   targetType
  , SkillTargetNature const   targetNature
  , ClockDuration     const   totalDuration
  , ClockDuration     const   tickDuration
  , ClockDuration     const   initialTriggerDuration
)
    : _type{type}
    , _skillTemplate{skillTemplate}
    , _targetType{targetType}
    , _targetNature{targetNature}
    , _totalDuration{totalDuration}
    , _tickDuration{tickDuration}
    , _initialTriggerDuration{initialTriggerDuration}
{}
