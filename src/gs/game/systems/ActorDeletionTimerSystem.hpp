/// @author    Chnossos
/// @date      Created on 2026-04-05

#pragma once

// Project includes
#include <gs/game/ecs/ActorSystem.hpp>

struct ActorDeletionTimerSystem : public ActorSystem
{
    void updateImpl(ClockDuration elapsed, Actor &) override;
};
