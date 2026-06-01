/// @author    Chnossos
/// @date      Created on 2026-05-06

#include "Auth.hpp"

// Project includes
#include "../utils/Conversion.hpp"

#include <l2cpp/services/Database.hpp>

// Third-party includes
#include <spdlog/spdlog.h>

auto Orm::fetchAccountId(std::wstring_view const accountName)
    -> std::optional<AccountId>
try
{
    static SQLite::Statement query(Database::instance(), "SELECT id FROM accounts WHERE name LIKE ?");
    query.reset();

    query.bind(1, Utils::toString(accountName));

    return query.executeStep() ? std::optional(query.getColumn(0).getUInt()) : std::nullopt;
}
catch (SQLite::Exception const & e)
{
    SPDLOG_ERROR("Failed to fetch account ID: SQL error '{}' (code: {})", e.what(), e.getErrorCode());
    return std::nullopt;
}
