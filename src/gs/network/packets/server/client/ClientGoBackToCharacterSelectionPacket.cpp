/// @author    Chnossos
/// @date      Created on 2026-03-31

#include "ClientGoBackToCharacterSelectionPacket.hpp"

using Network::Packets::Server::ClientGoBackToCharacterSelectionPacket;

ClientGoBackToCharacterSelectionPacket::ClientGoBackToCharacterSelectionPacket(bool const ok)
    : Packet(0x5f, "ClientGoBackToCharacterSelection")
{
    *this << static_cast<u32>(ok);
}
