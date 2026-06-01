/// @author    Chnossos
/// @date      Created on 2026-03-04

#pragma once

// Project includes
#include "../../Typedefs.hpp"

enum class ItemCategory : s16
{
    Unknown = -1,

    Weapon,
    Armor,
    Accessory,
    Quest,
    Adena,
    Misc,

    Count,
};
