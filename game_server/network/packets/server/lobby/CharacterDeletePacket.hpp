/// @author    Chnossos
/// @date      Created on 2026-06-01

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

namespace Network::Packet::Server { struct CharacterDeletePacket; }

enum class CharacterDeletionFailureReason : u32
{
    NoReason,
    GeneralFailure,
    YouAreClanMember,
    YouAreClanLeader,
};

struct Network::Packet::Server::CharacterDeletePacket final : public l2cpp::Network::Packet
{
    CharacterDeletePacket();
    explicit CharacterDeletePacket(CharacterDeletionFailureReason);
};
