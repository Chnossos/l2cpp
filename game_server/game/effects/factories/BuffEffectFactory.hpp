/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include "../../constants/StatId.hpp"
#include "AbnormalEffectFactory.hpp"

class BuffEffectFactory : public AbnormalEffectFactory
{
public:
    BuffEffectFactory(
        SkillTemplate     const & skillTemplate
      , EffectTargetType          targetType
      , SkillTargetNature         targetNature
      , ClockDuration             duration
      , StatId                    modifiedStat
      , StatValue                 value
    );

public:
    void apply(Actor & source, Actor & target) override;

private:
    StatId    _modifiedStat;
    StatValue _value;
};

