/// @author    Chnossos
/// @date      Created on 2026-04-12

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/ecs/Component.hpp>

struct ActorAutoRegen : public Component
{
    ClockDuration elapsedSinceLastUpdate = ClockDuration::zero();
};
