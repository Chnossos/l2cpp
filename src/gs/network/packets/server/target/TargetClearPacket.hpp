/// @author    Chnossos
/// @date      Created on 2026-04-05

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Character;

namespace Network::Packets::Server { struct TargetClearPacket; }

struct Network::Packets::Server::TargetClearPacket final : public Network::Packet
{
    explicit TargetClearPacket(Character const &);
};
