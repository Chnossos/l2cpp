/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "ResurrectionEffectFactory.hpp"

// Project includes
#include <gs/game/actor/Actor.hpp>
#include <gs/game/effects/ResurrectionEffect.hpp>
#include <gs/game/skill/SkillTemplate.hpp>

ResurrectionEffectFactory::ResurrectionEffectFactory(
    SkillTemplate     const & skillTemplate
  , EffectTargetType  const   targetType
  , SkillTargetNature const   targetNature
)
    : EffectFactory{EffectType::Resurrection, skillTemplate, targetType, targetNature}
{}

void ResurrectionEffectFactory::apply(Actor & source, Actor & target)
{
    target.addEffect<ResurrectionEffect>(source, target, skill().uid());
}
