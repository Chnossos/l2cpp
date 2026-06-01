/// @author    Chnossos
/// @date      Created on 2026-03-14

#pragma once

// Project includes
#include <l2cpp/core/Exception.hpp>

// C++ includes
#include <string_view>

namespace Utils
{
    namespace details
    {
        template<typename T, typename... Args>
        auto stringViewTo(std::string_view const str, Args &&... args) -> T
        {
            T val;
            auto const result = std::from_chars(str.data(), str.data() + str.size(), val, std::forward<Args>(args)...);
            L2CPP_B_ASSERT(result.ec == std::errc(), "Failed to convert '{}' into a number", str);
            return val;
        }
    }

    template<typename T> requires std::integral<T> || std::is_enum_v<T>
    auto stringViewTo(std::string_view const str, int const base = 10) -> T
    {
        if constexpr (std::is_enum_v<T>)
            return static_cast<T>(details::stringViewTo<std::underlying_type_t<T>>(str, base));
        else
            return details::stringViewTo<T>(str, base);
    }

    template<typename T> requires std::floating_point<T>
    auto stringViewTo(std::string_view const str) -> T { return details::stringViewTo<T>(str); }

    inline auto toString(std::wstring_view const wstr) -> std::string
    {
        std::string result(wstr.size(), '\0');

        auto const str = reinterpret_cast<byte const *>(wstr.data());
        for (size_t i = 0; i < wstr.size(); ++i)
            result[i] = str[i * 2];

        return result;
    }

    inline auto toWideString(std::string_view const str) -> std::wstring
    {
        std::wstring result(str.size(), L'\0');

        auto const wstr = reinterpret_cast<byte *>(result.data());
        for (size_t i = 0; i < str.size(); ++i)
            wstr[i * 2] = str[i];

        return result;
    }
}
