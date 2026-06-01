/// @author    Chnossos
/// @date      Created on 2026-04-05

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../ecs/Component.hpp"

struct DeletionTimer : public Component
{
    ClockDuration timeBeforeDeletion;
    ClockDuration elapsedSinceDeath;
};
