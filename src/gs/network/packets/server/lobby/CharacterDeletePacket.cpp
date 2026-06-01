/// @author    Chnossos
/// @date      Created on 2026-06-01

#include "CharacterDeletePacket.hpp"

using Network::Packets::Server::CharacterDeletePacket;

CharacterDeletePacket::CharacterDeletePacket()
    : Packet{0x23, "CharacterDeleteOk"}
{}

CharacterDeletePacket::CharacterDeletePacket(CharacterDeletionFailureReason const reason)
    : Packet{0x24, "CharacterDeleteFail"}
{
    *this << reason;
}
