/// @author    Chnossos
/// @date      Created on 2026-03-16

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Npc;

namespace Network::Packets::Server { struct NpcStatusUpdatePacket; }

struct Network::Packets::Server::NpcStatusUpdatePacket final : public Network::Packet
{
    explicit NpcStatusUpdatePacket(Npc const &);
};
