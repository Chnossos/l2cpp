/// @author    Chnossos
/// @date      Created on 2026-05-06

#pragma once

// Project includes
#include "../Typedefs.hpp"

namespace Orm
{
    auto fetchAccountId(std::wstring_view accountName) -> std::optional<AccountId>;
}
