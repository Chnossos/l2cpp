/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "HealEffectFactory.hpp"

// Project includes
#include "../../actor/Actor.hpp"
#include "../../skill/SkillTemplate.hpp"
#include "../HealEffect.hpp"

HealEffectFactory::HealEffectFactory(
    SkillTemplate     const & skillTemplate
  , EffectTargetType  const   targetType
  , SkillTargetNature const   targetNature
  , u32               const   power
  , ClockDuration     const   totalDuration
  , ClockDuration     const   tickDuration
  , ClockDuration     const   initialTriggerDuration
)
    : EffectFactory{
        EffectType::Damage, skillTemplate, targetType, targetNature,
        totalDuration, tickDuration, initialTriggerDuration
    }
    , _power{power}
{}

void HealEffectFactory::apply(Actor & source, Actor & target)
{
    target.addEffect<HealEffect>(source, target, skill().uid(), _power,
                                 totalDuration(), tickDuration(), initialTriggerDuration());
}
