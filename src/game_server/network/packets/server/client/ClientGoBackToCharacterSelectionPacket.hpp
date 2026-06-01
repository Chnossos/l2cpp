/// @author    Chnossos
/// @date      Created on 2026-03-31

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

namespace Network::Packets::Server { struct ClientGoBackToCharacterSelectionPacket; }

struct Network::Packets::Server::ClientGoBackToCharacterSelectionPacket final : public Network::Packet
{
    explicit ClientGoBackToCharacterSelectionPacket(bool ok);
};
