/// @author    Chnossos
/// @date      Created on 2026-06-11

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/Typedefs.hpp>

struct Position;

namespace Network::Packets::Server { struct ObjectTeleportPacket; }

enum class TeleportationStyle
{
    Fading,
    Instant,
};

struct Network::Packets::Server::ObjectTeleportPacket final : public Packet
{
    ObjectTeleportPacket(GameObjectId, Position const &, TeleportationStyle = TeleportationStyle::Fading);
};
