/// @author    Chnossos
/// @date      Created on 2026-03-25

#pragma once

// Project includes
#include "../ecs/ActorSystem.hpp"

struct ActorAttackStanceTimerSystem : public ActorSystem
{
    void updateImpl(ClockDuration elapsed, Actor &) override;
};
