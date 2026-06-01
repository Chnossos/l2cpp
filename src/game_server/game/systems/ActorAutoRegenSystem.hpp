/// @author    Chnossos
/// @date      Created on 2026-04-12

#pragma once

// Project includes
#include "../ecs/ActorSystem.hpp"

struct ActorAutoRegenSystem : public ActorSystem
{
    void updateImpl(ClockDuration elapsed, Actor &) override;
};
