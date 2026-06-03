/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include <gs/game/constants/StatId.hpp>
#include <gs/game/effects/Effect.hpp>

class BuffEffect : public Effect
{
public:
    BuffEffect(
        Actor         & source
      , Actor         & target
      , SkillUid        skillUid
      , ClockDuration   duration
      , StatId          modifiedStat
      , StatValue       value
    );

public:
    auto modifiedStat()      const -> StatId    { return _modifiedStat; }
    auto modifiedStatValue() const -> StatValue { return _value;        }

private:
    StatId    _modifiedStat;
    StatValue _value;
};

