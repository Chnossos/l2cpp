/// @author    Chnossos
/// @date      Created on 2026-06-11

#include "ObjectTeleportPacket.hpp"

// Project includes
#include <gs/game/components/Position.hpp>

using Network::Packets::Server::ObjectTeleportPacket;

ObjectTeleportPacket::ObjectTeleportPacket(GameObjectId       const   id,
                                           Position           const & position,
                                           TeleportationStyle const   style)
    : Packet(0x28, "ObjectTeleport")
{
    *this
        << id
        << position
        << style
        << position.orientation
    ;
}
