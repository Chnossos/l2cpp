/// @author    Chnossos
/// @date      Created on 2026-03-21

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct AttackStanceTogglePacket; }

struct Network::Packets::Server::AttackStanceTogglePacket final : public Network::Packet
{
    AttackStanceTogglePacket(bool enable, Actor const & target);
};
