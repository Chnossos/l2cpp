/// @author    Chnossos
/// @date      Created on 2026-04-09

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct ActorMoveStopPacket; }

struct Network::Packets::Server::ActorMoveStopPacket final : public Network::Packet
{
    explicit ActorMoveStopPacket(Actor const & actor);
};
