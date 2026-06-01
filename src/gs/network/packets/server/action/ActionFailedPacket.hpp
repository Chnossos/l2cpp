/// @author    Chnossos
/// @date      Created on 2026-04-21

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

namespace Network::Packets::Server { struct ActionFailedPacket; }

struct Network::Packets::Server::ActionFailedPacket final : public Network::Packet
{
    ActionFailedPacket();
};
