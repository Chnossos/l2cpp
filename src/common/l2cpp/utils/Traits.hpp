/// @author    Chnossos
/// @date      Created on 2026-03-14

#pragma once

// C++ includes
#include <type_traits>

namespace Utils::Traits
{
    template<typename T, typename Arg1, typename... ArgN>
    constexpr bool isAnyOf = (std::is_same_v<T, Arg1> || ... || std::is_same_v<T, ArgN>);
}
