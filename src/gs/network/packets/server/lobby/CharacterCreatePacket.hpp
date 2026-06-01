/// @author    Chnossos
/// @date      Created on 2026-05-06

#pragma once

// Project includes
#include "../../../../game/constants/CharacterCreationResult.hpp"

#include <common/network/Packet.hpp>

namespace Network::Packets::Server { struct CharacterCreatePacket; }

struct Network::Packets::Server::CharacterCreatePacket final : public Network::Packet
{
    explicit CharacterCreatePacket(CharacterCreationResult result);
};
