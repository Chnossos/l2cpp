/// @author    Chnossos
/// @date      Created on 2026-03-17

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Actor;
class Character;

namespace Network::Packets::Server { struct TargetSelectPacket; }

struct Network::Packets::Server::TargetSelectPacket final : public Network::Packet
{
    explicit TargetSelectPacket(Character const & source, Actor const & target);
};
