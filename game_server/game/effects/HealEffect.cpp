/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "HealEffect.hpp"

// Project includes
#include "../actor/Actor.hpp"

HealEffect::HealEffect(
    Actor               & source
  , Actor               & target
  , SkillUid      const   skillUid
  , StatValue     const   healAmount
  , ClockDuration const   effectDuration
  , ClockDuration const   tickDuration
  , ClockDuration const   initialTriggerDuration
)
    : AbnormalEffect(AbnormalEffectType::Heal, source, target, skillUid,
                     effectDuration, tickDuration, initialTriggerDuration)
    , _healAmount(healAmount)
{}

void HealEffect::onTick()
{
    target().takeDamage(source(), -_healAmount);
}

