/// @author    Chnossos
/// @date      Created on 2026-05-14

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class ShortcutBar;

namespace Network::Packets::Server { struct UiShortcutListPacket; }

struct Network::Packets::Server::UiShortcutListPacket final : public Network::Packet
{
    explicit UiShortcutListPacket(ShortcutBar const &);
};
