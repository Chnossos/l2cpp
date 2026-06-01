/// @author    Chnossos
/// @date      Created on 2026-03-04

#pragma once

// Project includes
#include "../constants/GearSlot.hpp"
#include "../constants/ItemCategory.hpp"
#include "../constants/ItemGrade.hpp"

// C++ includes
#include <string>

struct ItemTemplate
{
    u32          id = 0;
    std::string  name;
    ItemCategory category = ItemCategory::Unknown;
    ItemGrade    grade    = ItemGrade::None;
    GearSlot     gearSlot = GearSlot::None;
};
