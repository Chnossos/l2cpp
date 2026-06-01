/// @author    Chnossos
/// @date      Created on 2026-03-25

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../ecs/Component.hpp"

struct AttackStanceTimer : public Component
{
    ClockTimePoint startTime = std::chrono::steady_clock::now();
    ClockDuration  elapsedSinceStart;

    void restart()
    {
        startTime         = std::chrono::steady_clock::now();
        elapsedSinceStart = ClockDuration::zero();
    }
};
