/// @author    Chnossos
/// @date      Created on 2026-03-21

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/game/constants/ChatType.hpp>

class Actor;

namespace Network::Packets::Server { struct ChatNpcSayPacket; }

struct Network::Packets::Server::ChatNpcSayPacket final : public Packet
{
    ChatNpcSayPacket(Actor const & emitter, ChatType type, std::wstring_view const & message);
};
