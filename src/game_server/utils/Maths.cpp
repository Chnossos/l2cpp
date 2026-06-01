/// @author    Chnossos
/// @date      Created on 2026-04-04

#include "Maths.hpp"

// Project includes
#include "../game/components/Position.hpp"

auto Utils::Maths::distance(Position const & pos1, Position const & pos2) -> double
{
    return std::hypot(static_cast<double>(pos1.x - pos2.x),
                      static_cast<double>(pos1.y - pos2.y),
                      static_cast<double>(pos1.z - pos2.z));
}
