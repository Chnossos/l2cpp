/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "ResurrectionEffectFactory.hpp"

// Project includes
#include "../../actor/Actor.hpp"
#include "../../skill/SkillTemplate.hpp"
#include "../ResurrectionEffect.hpp"

ResurrectionEffectFactory::ResurrectionEffectFactory(
    SkillTemplate     const & skillTemplate
  , EffectTargetType  const   targetType
  , SkillTargetNature const   targetNature
)
    : AbnormalEffectFactory{AbnormalEffectType::Resurrection, skillTemplate, targetType, targetNature}
{}

void ResurrectionEffectFactory::apply(Actor & source, Actor & target)
{
    target.addAbnormalEffect<ResurrectionEffect>(source, target, _skillTemplate.uid());
}
