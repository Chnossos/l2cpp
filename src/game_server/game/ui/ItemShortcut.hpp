/// @author    Chnossos
/// @date      Created on 2026-05-18

#pragma once

// Project includes
#include "Shortcut.hpp"

struct Item;

class ItemShortcut : public Shortcut
{
public:
    explicit ItemShortcut(Index index, Item const &);

public:
    auto targetId() const -> u32 override;

private:
    void serializeImpl(Network::Packet &) const override;

private:
    ItemId       _itemId;
    GameObjectId _itemObjectId;
};
