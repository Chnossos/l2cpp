/// @author    Chnossos
/// @date      Created on 2026-04-22

// ReSharper disable CppNonExplicitConvertingConstructor
// ReSharper disable CppNonExplicitConversionOperator

#pragma once

// C++ includes
#include <utility>

template<typename E> requires std::is_enum_v<E>
class EnumMask
{
public:
    constexpr EnumMask(E const e) noexcept: _value{e} {}

public:
    constexpr auto operator|(E const other) const -> EnumMask { return _value | other; }
    constexpr auto operator&(E const other) const -> EnumMask { return _value & other; }

public:
    constexpr operator bool() const noexcept { return std::to_underlying(_value); }
    constexpr operator E()    const noexcept { return _value;                     }

    template<typename T> requires std::integral<T>
    constexpr operator T() const noexcept { return static_cast<T>(_value); }

private:
    E _value;
};

template<typename E> requires std::is_enum_v<E>
constexpr auto operator~(E const value) -> E
{
    return static_cast<E>(~std::to_underlying(value));
}

template<typename E> requires std::is_enum_v<E>
constexpr auto operator|(E const e1, E const e2) -> EnumMask<E>
{
    return static_cast<E>(std::to_underlying(e1) | std::to_underlying(e2));
}

template<typename E> requires std::is_enum_v<E>
constexpr auto operator&(E const e1, E const e2) -> EnumMask<E>
{
    return static_cast<E>(std::to_underlying(e1) & std::to_underlying(e2));
}
