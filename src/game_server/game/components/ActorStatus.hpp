/// @author    Chnossos
/// @date      Created on 2026-03-17

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../ecs/Component.hpp"

struct ActorStatus : public Component
{
    ~ActorStatus() override = 0;
    virtual auto level() const -> u32 = 0;
    virtual void setLevel(u32 level, double percent = 0) = 0;
};

inline ActorStatus::~ActorStatus() = default;
