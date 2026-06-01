/// @author    Chnossos
/// @date      Created on 2026-03-04

#pragma once

// Project includes
#include <gs/game/actor/Character.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/inventory/InventoryListPacket.hpp>

DEFINE_PACKET_HANDLER(InventoryOpen)
{
    player.connection().send(InventoryListPacket(true, player.currentCharacter()->inventory()));
}
