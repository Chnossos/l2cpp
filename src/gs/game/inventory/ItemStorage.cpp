/// @author    Chnossos
/// @date      Created on 2026-03-09

#include "ItemStorage.hpp"

// Project includes
#include <common/core/Exception.hpp>
// ReSharper disable once CppUnusedIncludeDirective
#include <common/details/Pimpl.hpp>
#include <gs/Typedefs.hpp>

// C++ includes
#include <ranges>
#include <unordered_map>

struct ItemStorage::Impl
{
    std::unordered_map<GameObjectId, Item> items;
};

template class Pimpl<ItemStorage::Impl>;

ItemStorage::ItemStorage() = default;
ItemStorage::ItemStorage(ItemStorage &&) noexcept = default;
ItemStorage & ItemStorage::operator=(ItemStorage &&) noexcept = default;
ItemStorage::~ItemStorage() = default;

auto ItemStorage::findIf(std::function<bool(Item const &)> const & predicate) const -> OptRef<Item const>
{
    OptRef<Item const> result;

    if (predicate)
    {
        for (auto const & item : _impl->items | std::views::values)
        {
            if (predicate(item))
            {
                result = item;
                break;
            }
        }
    }

    return result;
}

auto ItemStorage::item(GameObjectId const uid) -> OptRef<Item>
{
    auto const it = _impl->items.find(uid);
    return it != _impl->items.end() ? OptRef{it->second} : std::nullopt;
}

auto ItemStorage::item(GameObjectId const uid) const -> OptRef<Item const>
{
    auto const it = _impl->items.find(uid);
    return it != _impl->items.cend() ? OptRef<Item const>{it->second} : std::nullopt;
}

auto ItemStorage::item(ItemTemplate const & tmplate) -> std::vector<Ref<Item>>
{
    std::vector<Ref<Item>> result;

    for (auto & item : _impl->items | std::views::values)
    {
        if (item.tmplate.id == tmplate.id)
            result.emplace_back(item);
    }

    return result;
}

auto ItemStorage::item(ItemTemplate const & tmplate) const -> std::vector<Ref<Item const>>
{
    auto const v = const_cast<ItemStorage &>(*this).item(tmplate); // NOLINT(*-pro-type-const-cast)
    return {v.cbegin(), v.cend()};
}

auto ItemStorage::items() const -> std::vector<Ref<Item const>>
{
    std::vector<Ref<Item const>> result;
    result.append_range(_impl->items | std::views::values);
    return result;
}

auto ItemStorage::limit() const -> u16 { return 1000; /* TODO */ }

auto ItemStorage::add(Item && item) -> Item &
{
    auto const uid      = item.id();
    auto const [it, ok] = _impl->items.try_emplace(uid, std::move(item));
    L2CPP_B_ASSERT(ok, "Failed to insert item '{}' into storage", uid);
    return it->second;
}

auto ItemStorage::remove(Item const & item) -> Item
{
    auto const uid = item.id();
    L2CPP_B_ASSERT(_impl->items.contains(uid), "Cannot remove item '{}' from storage: not found", uid);

    auto result = std::move(_impl->items[uid]);
    _impl->items.erase(uid);
    return result;
}

void ItemStorage::clear() { _impl->items.clear(); }
