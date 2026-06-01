/// @author    Chnossos
/// @date      Created on 2026-03-25

#pragma once

// Project includes
#include "../../Typedefs.hpp"

class Entity;

struct System
{
    System() noexcept = default;
    System(System const &) noexcept = default;
    System & operator=(System const &) noexcept = default;
    System(System &&) noexcept = default;
    System & operator=(System &&) noexcept = default;
    virtual ~System() noexcept = default;

    virtual void update(ClockDuration elapsed, Entity &) = 0;
};
