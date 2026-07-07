/// @author    Chnossos
/// @date      Created on 2026-03-31

#pragma once

// Project includes
#include <common/network/Packet.hpp>

namespace Network::Packets::Server { struct ClientExitGamePacket; }

struct Network::Packets::Server::ClientExitGamePacket final : public Packet
{
    ClientExitGamePacket();
};
