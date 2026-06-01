/// @author    Chnossos
/// @date      Created on 2026-03-25

#pragma once

// Project includes
#include "../actor/Actor.hpp"
#include "System.hpp"

struct ActorSystem : public System
{
    void update(ClockDuration elapsed, Entity & e) final { updateImpl(elapsed, static_cast<Actor &>(e)); }
    virtual void updateImpl(ClockDuration elapsed, Actor &) = 0;
};
