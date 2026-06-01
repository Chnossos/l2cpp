/// @author    Chnossos
/// @date      Created on 2026-04-05

#pragma once

// Project includes
#include "../ecs/ActorSystem.hpp"

struct ActorDeletionTimerSystem : public ActorSystem
{
    void updateImpl(ClockDuration elapsed, Actor &) override;
};
