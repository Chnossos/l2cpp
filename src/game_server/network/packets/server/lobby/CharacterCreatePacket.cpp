/// @author    Chnossos
/// @date      Created on 2026-05-06

#include "CharacterCreatePacket.hpp"

using Network::Packets::Server::CharacterCreatePacket;

CharacterCreatePacket::CharacterCreatePacket(CharacterCreationResult const result)
    : Packet(result == CharacterCreationResult::Success ? 0x19 : 0x1a,
             result == CharacterCreationResult::Success ? "CharacterCreateOk" : "CharacterCreateFail")
{
    if (result != CharacterCreationResult::Success)
        *this << std::to_underlying(result);
}
