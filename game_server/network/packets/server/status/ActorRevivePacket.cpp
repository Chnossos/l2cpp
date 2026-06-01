/// @author    Chnossos
/// @date      Created on 2026-04-24

#include "ActorRevivePacket.hpp"

// Project includes
#include "../../../../game/actor/Actor.hpp"

using Network::Packets::Server::ActorRevivePacket;

ActorRevivePacket::ActorRevivePacket(Actor const & target)
    : Packet(0x07, "ActorRevive")
{
    *this
        << target.id()
    ;
}
