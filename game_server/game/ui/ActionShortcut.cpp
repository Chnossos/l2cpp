/// @author    Chnossos
/// @date      Created on 2026-05-18

#include "ActionShortcut.hpp"

// Project includes
#include <l2cpp/network/Packet.hpp>

ActionShortcut::ActionShortcut(Index const index, u32 const actionId)
    : Shortcut{index, ShortcutType::Action}
    , _actionId{actionId}
{}

auto ActionShortcut::targetId() const -> u32
{
    return _actionId;
}

void ActionShortcut::serializeImpl(Network::Packet & p) const
{
    p << _actionId;
}
