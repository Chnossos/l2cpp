/// @author    Chnossos
/// @date      Created on 2026-05-18

#pragma once

// Project includes
#include <gs/game/ui/Shortcut.hpp>

class ActionShortcut : public Shortcut
{
public:
    explicit ActionShortcut(Index index, u32 actionId);

public:
    auto targetId() const -> u32 override;

private:
    void serializeImpl(Network::Packet &) const override;

private:
    u32 _actionId;
};
