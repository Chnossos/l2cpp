/// @author    Chnossos
/// @date      Created on 2026-05-18

#include "ItemShortcut.hpp"

// Project includes
#include "../inventory/Item.hpp"

#include <l2cpp/network/Packet.hpp>

ItemShortcut::ItemShortcut(Index const index, Item const & item)
    : Shortcut{index, ShortcutType::Item}
    , _itemId{item.uid}
    , _itemObjectId{item.id()}
{}

auto ItemShortcut::targetId() const -> u32
{
    return static_cast<u32>(_itemId);
}

void ItemShortcut::serializeImpl(Network::Packet & p) const
{
    p << _itemObjectId;
}
