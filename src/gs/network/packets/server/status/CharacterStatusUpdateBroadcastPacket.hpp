/// @author    Chnossos
/// @date      Created on 2026-02-28

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Character;

namespace Network::Packets::Server { class CharacterStatusUpdateBroadcastPacket; }

class Network::Packets::Server::CharacterStatusUpdateBroadcastPacket final : public Packet
{
public:
    explicit CharacterStatusUpdateBroadcastPacket(Character const &);
};
