/// @author    Chnossos
/// @date      Created on 2026-03-16

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../ecs/Component.hpp"

struct ActorAppearance : public Component
{
    double collisionRadius = 0;
    double collisionHeight = 0;

    ~ActorAppearance() override = 0;
};

inline ActorAppearance::~ActorAppearance() = default;
