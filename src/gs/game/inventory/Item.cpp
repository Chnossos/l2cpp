/// @author    Chnossos
/// @date      Created on 2026-03-04

#include "Item.hpp"

// Project includes
#include <common/core/Exception.hpp>
#include <common/network/Packet.hpp>

namespace
{
    u16 typeFromCategory(ItemCategory const cat)
    {
        using enum ItemCategory;
        switch (cat)
        {
            case Weapon:
            case Accessory:
                return 0;

            case Armor:
                return 1;

            case Quest:
            case Adena:
            case Misc:
                return 4;

            default:
                L2CPP_THROW("Unknown item category '{}' for type conversion", std::to_underlying(cat));
        }
    }
}

Item::Item()
    : Item(++nextId)
{}

Item::Item(ItemId const id)
    : uid(id)
{}

Network::Packet & operator<<(Network::Packet & p, Item const & item)
{
    return p
        << typeFromCategory(item.tmplate.category)
        << item.id()
        << item.tmplate.id
        << item.quantity
        << item.tmplate.category
        << 0_u16 // ?
        << (item.equipped ? 1_u16 : 0_u16)
        << item.tmplate.gearSlot
        << static_cast<u16>(item.enchantLevel) // or pet level shown in control item
        << 0_u16 // pet name exists or not shown in control item
    ;
}

ItemId Item::nextId = 0;
