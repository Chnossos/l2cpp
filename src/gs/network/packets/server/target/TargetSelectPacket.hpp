/// @author    Chnossos
/// @date      Created on 2026-03-17

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Actor;
class Character;

namespace Network::Packets::Server { struct TargetSelectPacket; }

struct Network::Packets::Server::TargetSelectPacket final : public Packet
{
    explicit TargetSelectPacket(Character const & source, Actor const & target);
};
