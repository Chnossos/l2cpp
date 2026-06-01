/// @author    Chnossos
/// @date      Created on 2026-03-15

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../ecs/Component.hpp"

#include <l2cpp/Network/Serialization.hpp>

struct Position : public Component
{
    DECLARE_PACKET_SERIALIZATION_OPERATORS(Position);

    s32 x, y, z, orientation = 0;

    explicit Position(s32 const x_ = 0, s32 const y_ = 0, s32 const z_ = 0) noexcept
        : x(x_), y(y_), z(z_)
    {}
};
