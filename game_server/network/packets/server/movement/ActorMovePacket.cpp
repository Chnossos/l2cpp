/// @author    Chnossos
/// @date      Created on 2026-04-09

#include "ActorMovePacket.hpp"

// Project includes
#include "../../../../game/actor/Actor.hpp"
#include "../../../../game/components/Position.hpp"

using Network::Packets::Server::ActorMovePacket;

ActorMovePacket::ActorMovePacket(Actor const & actor, Position const & origin, Position const & destination)
    : Packet(0x01, "ActorMove")
{
    *this
        << actor.id()
        << destination
        << origin;
    ;
}
