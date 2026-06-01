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
    using Type = std::underlying_type_t<E>;

public:
    constexpr EnumMask(E const value)
        : _value(std::to_underlying(value))
    {}

public:
    constexpr bool     operator==(E const value) const { return _value == std::to_underlying(value);                 }
    constexpr EnumMask operator& (E const value) const { return static_cast<E>(_value  & std::to_underlying(value)); }
    constexpr EnumMask operator| (E const value) const { return static_cast<E>(_value  | std::to_underlying(value)); }
    constexpr EnumMask operator^ (E const value) const { return static_cast<E>(_value  ^ std::to_underlying(value)); }
    constexpr EnumMask operator~ ()              const { return static_cast<E>(~_value);                             }

    constexpr EnumMask & operator&=(E const value) { _value &= std::to_underlying(value); return *this; }
    constexpr EnumMask & operator|=(E const value) { _value |= std::to_underlying(value); return *this; }
    constexpr EnumMask & operator^=(E const value) { _value ^= std::to_underlying(value); return *this; }

    constexpr operator bool() const { return _value;                 }
    constexpr operator Type() const { return _value;                 }
    constexpr operator E()    const { return static_cast<E>(_value); }


private:
    Type _value;
};

template<typename E> EnumMask(E) -> EnumMask<E>;

template<typename E> requires std::is_enum_v<E>
constexpr auto operator|(E const value1, E const value2) -> EnumMask<E> {
    return EnumMask(value1) |= value2;
}

template<typename E> requires std::is_enum_v<E>
constexpr auto operator&(E const value1, E const value2) -> EnumMask<E> {
    return EnumMask(value1) &= value2;
}

template<typename E> requires std::is_enum_v<E>
constexpr auto operator^(E const value1, E const value2) -> EnumMask<E> {
    return EnumMask(value1) ^= value2;
}

template<typename E> requires std::is_enum_v<E>
constexpr auto operator~(E const value) -> E {
    return static_cast<E>(~std::to_underlying(value));
}
