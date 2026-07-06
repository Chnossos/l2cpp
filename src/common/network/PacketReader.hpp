/// @author    Chnossos
/// @date      Created on 2026-02-23

#pragma once

// Project includes
#include <common/Typedefs.hpp>

// C++ includes
#include <cstring>
#include <span>
#include <string>

namespace Network { class PacketReader; }

class Network::PacketReader
{
public:
    explicit PacketReader(std::span<byte const> packet) noexcept: cursor(std::move(packet)) {}

public:
    template<typename T> requires std::integral<T> || std::floating_point<T> || std::is_enum_v<T>
    PacketReader & operator>>(T & t)
    {
        if constexpr (std::is_enum_v<T>) // TODO: better handling of overflow values
            t = static_cast<T>(*reinterpret_cast<std::underlying_type_t<T> const *>(cursor.data()));
        else
            t = *reinterpret_cast<T const *>(cursor.data());

        cursor = cursor.subspan(sizeof(T));
        return *this;
    }

    template<typename C> requires Utils::Traits::isAnyOf<C, char, wchar_t, char8_t, char16_t, char32_t>
    PacketReader & operator>>(std::basic_string<C> & str)
    {
        str = reinterpret_cast<C const *>(cursor.data());
        cursor = cursor.subspan(str.size() * sizeof(C) + sizeof(C));
        return *this;
    }

    template<typename T, size_t N>
    PacketReader & operator>>(std::array<T, N> & arr)
    {
        auto const data = cursor.subspan(0, N * sizeof(T));
        std::memcpy(arr.data(), data.data(), data.size());
        cursor = cursor.subspan(data.size());
        return *this;
    }

private:
    std::span<byte const> cursor;
};
