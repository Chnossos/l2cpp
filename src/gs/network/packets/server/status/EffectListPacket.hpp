/// @author    Chnossos
/// @date      Created on 2026-04-16

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct EffectListPacket; }

struct Network::Packets::Server::EffectListPacket final : public Network::Packet
{
    explicit EffectListPacket(Actor const &);
};
