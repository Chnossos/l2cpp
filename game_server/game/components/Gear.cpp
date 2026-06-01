/// @author    Chnossos
/// @date      Created on 2026-03-09

#include "Gear.hpp"

// Project includes
#include <l2cpp/Exception.hpp>
#include <l2cpp/details/Pimpl.hpp>

// C++ includes
#include <ranges>
#include <unordered_map>

struct Gear::Impl
{
public:
    auto itemAt(GearSlot slot)       -> OptRef<Item> &;
    auto itemAt(GearSlot slot) const -> OptRef<Item const>;

    /// Apply a function to a slot and all its dependent slots
    template<typename Func>
    auto applyToDependentSlots(GearSlot slot, Func apply);

    auto equipItem  (Item &) -> GearTransaction;
    auto unequipItem(Item const &, OptRef<GearTransaction> const & = std::nullopt) -> GearTransaction;

private:
    std::unordered_map<GearSlot, OptRef<Item>> gear;
};

template class Pimpl<Gear::Impl>;

auto Gear::Impl::itemAt(GearSlot const slot) -> OptRef<Item> &
{
    L2CPP_B_ASSERT(slot != GearSlot::None, "Tried to reference gear item @ slot None");
    switch (slot)
    {
        case GearSlot::Hands:         return gear[GearSlot::RightHand];
        case GearSlot::Fingers:       return gear[GearSlot::RightFinger];
        case GearSlot::TwoPieceArmor: return gear[GearSlot::Chest];
        case GearSlot::Dress:         return gear[GearSlot::Chest];
        default:                      return gear[slot];
    }
}

auto Gear::Impl::itemAt(GearSlot const slot) const -> OptRef<Item const>
{
    return *const_cast<Impl *>(this)->itemAt(slot);
}

template<typename Func>
auto Gear::Impl::applyToDependentSlots(GearSlot slot, Func apply)
{
    std::vector<GearSlot> slots;
    slots.reserve(4);

    switch (slot)
    {
        using enum GearSlot;
        case None:                                                                                break;
        case Hands:         slots.append_range(std::initializer_list{LeftHand, RightHand});       break;
        case Fingers:       slots.append_range(std::initializer_list{LeftFinger, RightFinger});   break;
        case TwoPieceArmor: slots.append_range(std::initializer_list{Chest, Legs});               break;
        case Dress:         slots.append_range(std::initializer_list{Chest, Legs, Gloves, Feet}); break;
        default:            slots.emplace_back(slot);                                             break;
    }

    for (auto const s : slots)
        std::invoke(apply, gear[s]);
}

auto Gear::Impl::equipItem(Item & item) -> GearTransaction
{
    GearTransaction transaction {.target = item};

    auto const slot = item.tmplate.gearSlot;
    if (slot == GearSlot::None) // Unequippable item
    {
        transaction.succeeded = false;
        return transaction;
    }

    if (item.tmplate.id == 1345 /* AND weapon type is matching grade bow*/) // FIXME: if is arrows of matching grade
    {
        gear[GearSlot::LeftHand] = item;
        item.equipped = true;
        transaction.curItem = item;
        return transaction;
    }

    // TODO: verify whether conditions are met to avoid unequipping when equipping is not possible

    if (auto const & curItemRef = itemAt(slot))
    {
        // If current item is arrows, unequip the bow instead
        Ref const curItem = curItemRef->tmplate.id == 1345 ? *itemAt(GearSlot::RightHand) : *curItemRef;

        // First, unequip all slots used by the item currently located where we want to equip.
        // This is needed in case the new item uses fewer slots than the current item.
        // E.g.: putting on gloves (1 slot) should remove Formal Wear (4 slots).
        applyToDependentSlots(curItem.get().tmplate.gearSlot, [this, &transaction] (OptRef<Item> const & itemSlot)
        {
            if (itemSlot)
                unequipItem(*itemSlot, transaction);
        });
    }

    // Second, also unequip all slots our new item is going to equip to.
    // That's needed when the new item uses more slots than the current item located where we want to equip.
    // E.g.: Formal Wear occupying multiple armor slots.
    applyToDependentSlots(slot, [this, &transaction] (OptRef<Item> const & itemSlot)
    {
        if (itemSlot)
            unequipItem(*itemSlot, transaction);
    });

    if (transaction.succeeded)
    {
        // Everything went right. Now truly unequip old items
        for (auto const & itemRef : transaction.oldItems | std::views::values)
        {
            applyToDependentSlots(itemRef.get().tmplate.gearSlot, [] (OptRef<Item> & itemSlot)
            {
                if (itemSlot)
                {
                    itemSlot->equipped = false;
                    itemSlot.reset();
                }
            });
        }

        // Finally, equip the item onto all dependent slots
        applyToDependentSlots(slot, [&item] (OptRef<Item> & itemSlot) { itemSlot = item; });
        item.equipped = true;
        transaction.curItem = item;
    }

    return transaction;
}

auto Gear::Impl::unequipItem(Item const & item, OptRef<GearTransaction> const & curTransaction)
    -> GearTransaction
{
    GearTransaction t{.target = item};
    auto & transaction = curTransaction ? *curTransaction : t;

    // First, verify that provided item is the one that's currently equipped
    auto const   slot    = item.tmplate.gearSlot;
    auto const & curItem = itemAt(slot);
    transaction.succeeded = transaction.succeeded // Do not proceed if current transaction has already failed
                            && curItem && curItem->id() == item.id();

    if (!transaction.succeeded)
        return transaction;

    // TODO: verify whether conditions are met before unequipping

    applyToDependentSlots(slot, [&transaction] (OptRef<Item> const & itemSlot)
    {
        if (itemSlot)
            transaction.oldItems.try_emplace(itemSlot->id(), *itemSlot);
    });

    // Now that we're about to finish with the transaction, apply if everything succeeded and we're not a subtransaction
    if (!curTransaction)
    {
        for (auto const & itemRef : transaction.oldItems | std::views::values)
        {
            applyToDependentSlots(itemRef.get().tmplate.gearSlot, [] (OptRef<Item> & itemSlot)
            {
                if (itemSlot)
                {
                    itemSlot->equipped = false;
                    itemSlot.reset();
                }
            });
        }
    }

    return transaction;
}

// ---------------------------------------------------------------------------------------------------------------------

Gear::Gear() = default;
Gear::Gear(Gear &&) noexcept = default;
Gear & Gear::operator=(Gear &&) noexcept = default;
Gear::~Gear() = default;

auto Gear::item(GearSlot const slot)       -> OptRef<Item>       { return _impl->itemAt(slot); }
auto Gear::item(GearSlot const slot) const -> OptRef<Item const> { return _impl->itemAt(slot); }

auto Gear::itemId(GearSlot const slot) const -> u32
{
    auto const i = item(slot);
    return i ? i->id() : 0;
}

auto Gear::itemTemplateId(GearSlot const slot) const -> u32
{
    auto const i = item(slot);
    return i ? i->tmplate.id : 0;
}

bool Gear::hasActiveWeapon() const { return weapon().has_value(); }
auto Gear::weapon() const -> OptRef<Item const> { return item(GearSlot::RightHand); }

auto Gear::equipItem  (Item &       item) -> GearTransaction { return _impl->equipItem(item);   }
auto Gear::unequipItem(Item const & item) -> GearTransaction { return _impl->unequipItem(item); }
