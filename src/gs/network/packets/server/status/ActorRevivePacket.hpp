/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct ActorRevivePacket; }

struct Network::Packets::Server::ActorRevivePacket final : public Network::Packet
{
    explicit ActorRevivePacket(Actor const & target);
};
