/// @author    Chnossos
/// @date      Created on 2026-03-04

#pragma once

// Project includes
#include "../GameObject.hpp"
#include "ItemTemplate.hpp"

#include <l2cpp/network/Serialization.hpp>

struct Item : public GameObject
{
    DECLARE_PACKET_SERIALIZATION_OPERATOR(Item);

    Item();
    explicit Item(ItemId);

    ItemTemplate tmplate;

    ItemId uid;
    u32    quantity     = 1;
    u8     enchantLevel = 0;
    bool   equipped     = false;

    static ItemId nextId;
};
