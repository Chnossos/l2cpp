/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "DamageEffectFactory.hpp"

// Project includes
#include "../../actor/Actor.hpp"
#include "../../skill/SkillTemplate.hpp"
#include "../DamageEffect.hpp"

DamageEffectFactory::DamageEffectFactory(
    SkillTemplate     const & skillTemplate
  , EffectTargetType  const   targetType
  , SkillTargetNature const   targetNature
  , DamageElementType const   elementType
  , u32               const   power
  , ClockDuration     const   totalDuration
  , ClockDuration     const   tickDuration
  , ClockDuration     const   initialTriggerDuration
)
    : EffectFactory{EffectType::Damage, skillTemplate, targetType, targetNature,
                    totalDuration, tickDuration, initialTriggerDuration}
    , _elementType{elementType}
    , _power{power}
{}

void DamageEffectFactory::apply(Actor & source, Actor & target)
{
    target.addEffect<DamageEffect>(source, target, skill().uid(), _elementType, _power,
                                   totalDuration(), tickDuration(), initialTriggerDuration());
}
