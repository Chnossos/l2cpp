/// @author    Chnossos
/// @date      Created on 2026-03-09

#pragma once

// Project includes
#include <common/Pimpl.hpp>
#include <gs/game/ecs/Component.hpp>
#include <gs/game/inventory/GearTransaction.hpp>

/// Handles the items that the character currently wears
class Gear : public Component
{
public:
    Gear();
    Gear(Gear const &) noexcept = delete;
    Gear & operator=(Gear const &) noexcept = delete;
    Gear(Gear &&) noexcept;
    Gear & operator=(Gear &&) noexcept;
    ~Gear() override;

public:
    auto item(GearSlot slot)       -> OptRef<Item>;
    auto item(GearSlot slot) const -> OptRef<Item const>;

    auto itemId        (GearSlot slot) const -> u32;
    auto itemTemplateId(GearSlot slot) const -> u32;

    bool hasActiveWeapon() const;
    auto weapon() const -> OptRef<Item const>;

public:
    auto equipItem  (Item       & item) -> GearTransaction;
    auto unequipItem(Item const & item) -> GearTransaction;

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
