/// @author    Chnossos
/// @date      Created on 2026-05-06

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/game/constants/CharacterCreationResult.hpp>

namespace Network::Packets::Server { struct CharacterCreatePacket; }

struct Network::Packets::Server::CharacterCreatePacket final : public Packet
{
    explicit CharacterCreatePacket(CharacterCreationResult result);
};
