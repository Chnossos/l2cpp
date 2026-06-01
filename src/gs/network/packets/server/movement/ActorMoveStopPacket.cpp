/// @author    Chnossos
/// @date      Created on 2026-04-09

#include "ActorMoveStopPacket.hpp"

// Project includes
#include "../../../../game/actor/Actor.hpp"
#include "../../../../game/components/Position.hpp"

using Network::Packets::Server::ActorMoveStopPacket;

ActorMoveStopPacket::ActorMoveStopPacket(Actor const & actor)
    : Packet(0x47, "ActorMoveStop")
{
    *this
        << actor.id()
        << actor.position()
        << 0 // orientation
    ;
}
