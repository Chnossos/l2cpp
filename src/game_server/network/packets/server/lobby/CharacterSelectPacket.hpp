/// @author    Chnossos
/// @date      Created on 2026-03-31

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Character;

namespace Network::Packets::Server { struct CharacterSelectPacket; }

struct Network::Packets::Server::CharacterSelectPacket final : public Network::Packet
{
    CharacterSelectPacket(Character const & c, u32 encryptionKey);
};
