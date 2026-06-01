/// @author    Chnossos
/// @date      Created on 2026-03-06

#include "InventoryListPacket.hpp"

using Network::Packets::Server::InventoryListPacket;

InventoryListPacket::InventoryListPacket(bool const openInventory, ItemStorage const & inventory)
    : Packet(0x1b, "InventoryList")
{
    auto const & items = inventory.items();

    *this
        << static_cast<u16>(openInventory)
        << static_cast<u16>(items.size())
    ;

    for (auto const & item : items)
        *this << item.get();
}
