/// @author    Chnossos
/// @date      Created on 2026-03-06

#pragma once

// Project includes
#include "../../../../game/inventory/ItemStorage.hpp"

#include <common/network/Packet.hpp>

namespace Network::Packets::Server { struct InventoryListPacket; }

struct Network::Packets::Server::InventoryListPacket final : public Network::Packet
{
    InventoryListPacket(bool openInventory, ItemStorage const & inventory);
};
