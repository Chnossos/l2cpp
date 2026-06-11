/// @author    Chnossos
/// @date      Created on 2026-03-15

#pragma once

// Project includes
#include <common/Network/Serialization.hpp>
#include <gs/Typedefs.hpp>
#include <gs/game/ecs/Component.hpp>

struct Position : public Component
{
    DECLARE_PACKET_SERIALIZATION_OPERATORS(Position);

    s32 x, y, z;
    u16 orientation;

    explicit Position(s32 const x_ = 0, s32 const y_ = 0, s32 const z_ = 0, u16 const orientation_ = 0) noexcept
        : x{x_}
        , y{y_}
        , z{z_}
        , orientation{orientation_}
    {}
};

inline bool operator==(Position const & lhs, Position const & rhs) noexcept
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}
