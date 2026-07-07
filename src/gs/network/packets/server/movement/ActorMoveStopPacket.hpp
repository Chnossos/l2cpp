/// @author    Chnossos
/// @date      Created on 2026-04-09

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct ActorMoveStopPacket; }

struct Network::Packets::Server::ActorMoveStopPacket final : public Packet
{
    explicit ActorMoveStopPacket(Actor const & actor);
};
