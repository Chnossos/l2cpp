/// @author    Chnossos
/// @date      Created on 2026-03-06

#pragma once

// Project includes
#include "../../../../game/inventory/Item.hpp"

#include <common/network/Packet.hpp>

namespace Network::Packets::Server { struct InventoryUpdatePacket; }

struct Network::Packets::Server::InventoryUpdatePacket final : public Network::Packet
{
    InventoryUpdatePacket();

    void appendAddedItem   (Item const &);
    void appendModifiedItem(Item const &);
    void appendRemovedItem (Item const &);
};
