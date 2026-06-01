/// @author    Chnossos
/// @date      Created on 2026-03-04

#pragma once

// Project includes
#include <gs/game/constants/GearSlot.hpp>
#include <gs/game/constants/ItemCategory.hpp>
#include <gs/game/constants/ItemGrade.hpp>

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
