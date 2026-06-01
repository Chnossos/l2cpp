/// @author    Chnossos
/// @date      Created on 2026-06-01

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

namespace Network::Packets::Server { struct CharacterDeletePacket; }

enum class CharacterDeletionFailureReason : u32
{
    NoReason,
    GeneralFailure,
    YouAreClanMember,
    YouAreClanLeader,
};

struct Network::Packets::Server::CharacterDeletePacket final : public Network::Packet
{
    CharacterDeletePacket();
    explicit CharacterDeletePacket(CharacterDeletionFailureReason);
};
