/// @author    Chnossos
/// @date      Created on 2026-04-27

#pragma once

// C++ includes
#include "../../Typedefs.hpp"
#include "../ecs/Component.hpp"

struct Loot : public Component
{
    u32 xp = 0;
    u32 sp = 0;
};
