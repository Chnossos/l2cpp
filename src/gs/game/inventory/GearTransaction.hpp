/// @author    Chnossos
/// @date      Created on 2026-03-09

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../inventory/Item.hpp"

struct GearTransaction
{
    bool succeeded = true;      ///< Whether equip/unequip action has worked
    OptRef<Item const> target;  ///< Item that the user wants to equip/unequip
    OptRef<Item const> curItem; ///< Item that has been successfully equipped or nullopt if unequip successful

    /// Items that were in the slots prior to the transaction
    std::unordered_map<GameObjectId, Ref<Item const>> oldItems;
};
