/// @author    Chnossos
/// @date      Created on 2026-04-04

#pragma once

// Project includes
#include "../game/actor/Actor.hpp"

namespace Utils::Maths
{
    auto distance(Position const & pos1, Position const & pos2) -> double;
    inline auto distance(Actor const & a1, Actor const & a2) { return distance(a1.position(), a2.position()); }
}
