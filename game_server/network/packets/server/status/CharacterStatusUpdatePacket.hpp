/// @author    Chnossos
/// @date      Created on 2026-02-28

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Character;

namespace Network::Packets::Server { class CharacterStatusUpdatePacket; }

class Network::Packets::Server::CharacterStatusUpdatePacket final : public Network::Packet
{
public:
    explicit CharacterStatusUpdatePacket(Character const &);
};
