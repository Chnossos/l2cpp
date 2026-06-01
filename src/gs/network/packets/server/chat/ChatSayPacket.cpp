/// @author    Chnossos
/// @date      Created on 2026-03-04

#include "ChatSayPacket.hpp"

using Network::Packets::Server::ChatSayPacket;

ChatSayPacket::ChatSayPacket(u32 const senderId,  std::wstring_view const sender,
                             ChatType const type, std::wstring_view const message)
    : Packet(0x4a, "ChatSay")
{
    *this
        << senderId
        << type
        << sender
        << message
    ;
}
