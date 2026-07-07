/// @author    Chnossos
/// @date      Created on 2026-03-06

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/game/inventory/Item.hpp>

namespace Network::Packets::Server { struct InventoryUpdatePacket; }

struct Network::Packets::Server::InventoryUpdatePacket final : public Packet
{
    InventoryUpdatePacket();

    void appendAddedItem   (Item const &);
    void appendModifiedItem(Item const &);
    void appendRemovedItem (Item const &);
};
