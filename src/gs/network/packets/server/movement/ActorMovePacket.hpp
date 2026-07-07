/// @author    Chnossos
/// @date      Created on 2026-04-09

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Actor;
struct Position;

namespace Network::Packets::Server { struct ActorMovePacket; }

struct Network::Packets::Server::ActorMovePacket final : public Packet
{
    ActorMovePacket(Actor const & actor, Position const & origin, Position const & destination);
};
