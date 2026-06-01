/// @author    Chnossos
/// @date      Created on 2026-04-16

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Actor;

namespace Network::Packet::Server { struct EffectListPacket; }

struct Network::Packet::Server::EffectListPacket final : public l2cpp::Network::Packet
{
    explicit EffectListPacket(Actor const &);
};
