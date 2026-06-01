/// @author    Chnossos
/// @date      Created on 2026-04-03

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct ActorDiePacket; }

struct Network::Packets::Server::ActorDiePacket final : public Network::Packet
{
    explicit ActorDiePacket(Actor const &);
};
