/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "BuffEffect.hpp"

BuffEffect::BuffEffect(
    Actor               & source
  , Actor               & target
  , SkillUid      const   skillUid
  , ClockDuration const   duration
  , StatId        const   modifiedStat
  , StatValue     const   value
)
    : Effect{EffectType::Buff, source, target, skillUid, duration}
    , _modifiedStat{modifiedStat}
    , _value{value}
{}
