/// @author    Chnossos
/// @date      Created on 2026-05-14

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Shortcut;

namespace Network::Packets::Server { struct UiShortcutSetPacket; }

struct Network::Packets::Server::UiShortcutSetPacket final : public Network::Packet
{
    explicit UiShortcutSetPacket(Shortcut const &);
};
