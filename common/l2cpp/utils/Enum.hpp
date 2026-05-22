/// @author    Chnossos
/// @date      Created on 2026-03-05

#pragma once

// C++ includes
#include <utility>

namespace l2cpp::Utils::Enum
{
    template<typename E, typename Func> requires std::is_enum_v<E> && std::invocable<Func, E>
    constexpr void foreach(E begin, E end, Func f)
    {
        for (auto b = std::to_underlying(begin), e = std::to_underlying(end); b < e; ++b)
            f(static_cast<E>(b));
    }

    template<typename E> requires std::is_enum_v<E>
    constexpr bool isInContiguousRange(E index, E start, E end)
    {
        return std::to_underlying(index) >= std::to_underlying(start)
            && std::to_underlying(index)  < std::to_underlying(end);
    }

    template<typename E> requires std::is_enum_v<E>
    constexpr bool isInContiguousInclusiveRange(E index, E start, E end)
    {
        return std::to_underlying(index) >= std::to_underlying(start)
            && std::to_underlying(index) <= std::to_underlying(end);
    }

    template<typename E, typename... Args> requires (std::is_enum_v<E> && ... && std::is_same_v<E, Args>)
    constexpr bool isAnyOf(E value, E arg1, Args... args)
    {
        return ((value == arg1) || ... || (value == std::forward<Args>(args)));
    }
}
