/// @author    Chnossos
/// @date      Created on 2026-03-21

#pragma once

// Project includes
#include "../../../../game/constants/ChatType.hpp"

#include <l2cpp/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct ChatNpcSayPacket; }

struct Network::Packets::Server::ChatNpcSayPacket final : public Network::Packet
{
    ChatNpcSayPacket(Actor const & emitter, ChatType type, std::wstring_view const & message);
};
