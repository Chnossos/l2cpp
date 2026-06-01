/// @author    Chnossos
/// @date      Created on 2026-05-14

#include "UiShortcutListPacket.hpp"

// Project includes
#include <gs/game/ui/ShortcutBar.hpp>

// C++ includes
#include <algorithm>

using Network::Packets::Server::UiShortcutListPacket;

UiShortcutListPacket::UiShortcutListPacket(ShortcutBar const & shortcutBar)
    : Packet(0x45, "UiShortcutList")
{
    auto const shortcuts = shortcutBar.shortcuts();

    *this
        << static_cast<u32>(std::ranges::count_if(shortcuts, [] (auto const & s) { return !!s; }))
    ;

    for (auto const & shortcut : shortcuts)
    {
        if (shortcut)
            *this << *shortcut;
    }
}
