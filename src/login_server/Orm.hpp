/// @author    Chnossos
/// @date      Created on 2026-05-07

#pragma once

// Project includes
#include <l2cpp/Typedefs.hpp>

// C++ includes
#include <string_view>
#include <vector>

namespace Orm
{
    auto fetchAccountPassword(std::string_view accountName) -> std::optional<std::vector<byte>>;
    void createAccount(std::string_view accountName, std::string_view password);
}
