/// @author    Chnossos
/// @date      Created on 2026-03-09

#pragma once

// Project includes
#include "Item.hpp"

#include <l2cpp/Pimpl.hpp>

// C++ includes
#include <functional>
#include <vector>

/// Could be a player/pet inventory, a wharehouse, etc.
class ItemStorage
{
public:
    ItemStorage();
    ItemStorage(ItemStorage &&) noexcept;
    ItemStorage & operator=(ItemStorage &&) noexcept;
    virtual ~ItemStorage();

public:
    auto find_if(std::function<bool(Item const &)> const & predicate) const -> OptRef<Item const>;

    auto item(GameObjectId uid)           -> OptRef<Item>;
    auto item(GameObjectId uid)     const -> OptRef<Item const>;
    auto item(ItemTemplate const &)       -> std::vector<Ref<Item>>;
    auto item(ItemTemplate const &) const -> std::vector<Ref<Item const>>;
    auto items()                    const -> std::vector<Ref<Item const>>;
    auto limit()                    const -> u16;

public:
    auto add(Item && item) -> Item &;

    /// @return Gives a chance to take back ownership of the item instance, else it gets destroyed.
    auto remove(Item const &) -> Item;

    void clear();

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
