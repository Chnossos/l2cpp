/// @author    Chnossos
/// @date      Created on 2026-04-24

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct ActorRevivePacket; }

struct Network::Packets::Server::ActorRevivePacket final : public Packet
{
    explicit ActorRevivePacket(Actor const & target);
};
