/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "BuffEffectFactory.hpp"

// Project includes
#include "../../actor/Actor.hpp"
#include "../../skill/SkillTemplate.hpp"
#include "../BuffEffect.hpp"

BuffEffectFactory::BuffEffectFactory(
    SkillTemplate     const & skillTemplate
  , EffectTargetType  const   targetType
  , SkillTargetNature const   targetNature
  , ClockDuration     const   duration
  , StatId            const   modifiedStat
  , StatValue         const   value
)
    : EffectFactory{EffectType::Buff, skillTemplate, targetType, targetNature, duration}
    , _modifiedStat{modifiedStat}
    , _value{value}
{}

void BuffEffectFactory::apply(Actor & source, Actor & target)
{
    if (source.type() == ActorType::Character && target.type() != ActorType::Character)
        return;

    target.addEffect<BuffEffect>(source, target, skill().uid(), totalDuration(), _modifiedStat, _value);
}
