/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include "Effect.hpp"

class ResurrectionEffect : public Effect
{
public:
    ResurrectionEffect(Actor & source, Actor & target, SkillUid skillUid);

private:
    void onStarted() override;
};
