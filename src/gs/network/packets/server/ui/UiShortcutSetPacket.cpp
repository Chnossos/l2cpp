/// @author    Chnossos
/// @date      Created on 2026-05-14

#include "UiShortcutSetPacket.hpp"

// Project includes
#include <gs/game/ui/Shortcut.hpp>

using Network::Packets::Server::UiShortcutSetPacket;

UiShortcutSetPacket::UiShortcutSetPacket(Shortcut const & shortcut)
    : Packet(0x44, "UiShortcutSet")
{
    *this << shortcut;
}
