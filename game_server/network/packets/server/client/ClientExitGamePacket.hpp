/// @author    Chnossos
/// @date      Created on 2026-03-31

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

namespace Network::Packets::Server { struct ClientExitGamePacket; }

struct Network::Packets::Server::ClientExitGamePacket final : public Network::Packet
{
    ClientExitGamePacket();
};
