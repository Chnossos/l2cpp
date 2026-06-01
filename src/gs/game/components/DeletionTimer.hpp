/// @author    Chnossos
/// @date      Created on 2026-04-05

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/ecs/Component.hpp>

struct DeletionTimer : public Component
{
    ClockDuration timeBeforeDeletion;
    ClockDuration elapsedSinceDeath;
};
