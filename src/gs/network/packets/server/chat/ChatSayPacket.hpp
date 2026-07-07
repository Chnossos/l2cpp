/// @author    Chnossos
/// @date      Created on 2026-03-04


#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/game/constants/ChatType.hpp>

namespace Network::Packets::Server { struct ChatSayPacket; }

struct Network::Packets::Server::ChatSayPacket final : public Packet
{
    ChatSayPacket(u32 senderId, std::wstring_view sender, ChatType type, std::wstring_view message);
};
