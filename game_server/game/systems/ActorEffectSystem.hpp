/// @author    Chnossos
/// @date      Created on 2026-04-14

#pragma once

// Project includes
#include "../ecs/ActorSystem.hpp"

struct ActorEffectSystem : public ActorSystem
{
    void updateImpl(ClockDuration elapsed, Actor &) override;
};
