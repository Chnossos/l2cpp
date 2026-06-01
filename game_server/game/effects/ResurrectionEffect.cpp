/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "ResurrectionEffect.hpp"

// Project includes
#include "../actor/Character.hpp"

ResurrectionEffect::ResurrectionEffect(Actor & source, Actor & target, SkillUid const skillUid)
    : Effect{EffectType::Resurrection, source, target, skillUid}
{}

void ResurrectionEffect::onStarted()
{
    if (target().type() == ActorType::Character)
        static_cast<Character &>(target()).offerResurrection(source());
}
