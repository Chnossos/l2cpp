/// @author    Chnossos
/// @date      Created on 2026-05-18

#pragma once

// Project includes
#include "Shortcut.hpp"

// C++ includes
#include <array>
#include <span>

class ShortcutBar
{
    static constexpr size_t maxShortcuts = 120;

public:
    ShortcutBar() = default;

public:
    auto size()              const -> size_t { return maxShortcuts; }
    auto at(Shortcut::Index) const -> OptRef<Shortcut>;
    auto shortcuts()         const -> std::span<OptRef<Shortcut const> const, maxShortcuts>;

public:
    template<class T> requires std::is_base_of_v<Shortcut, T>
    void set(Shortcut::Index const index, auto &&... args)
    {
        checkIndex(index);
        setImpl(index, std::make_unique<T>(static_cast<u32>(index), std::forward<decltype(args)>(args)...));
    }

    void unset(Shortcut::Index);
    void unsetAll();

private:
    void checkIndex(Shortcut::Index) const;
    void setImpl(Shortcut::Index, std::unique_ptr<Shortcut>);

private:
    std::array<std::unique_ptr<Shortcut>, maxShortcuts> _shortcuts;
    std::array<OptRef<Shortcut const>,    maxShortcuts> _readOnlyShortcuts;
};
