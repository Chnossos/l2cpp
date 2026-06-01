/// @author    Chnossos
/// @date      Created on 2026-03-07

#pragma once

// Project includes
#include <common/network/Packet.hpp>

namespace Network::Packets::Server { struct ClientForceDisconnectPacket; }

struct Network::Packets::Server::ClientForceDisconnectPacket final : public Network::Packet
{
    ClientForceDisconnectPacket();
};
