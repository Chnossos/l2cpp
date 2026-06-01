/// @author    Chnossos
/// @date      Created on 2026-05-07

#include "Orm.hpp"

// Project includes
#include <l2cpp/services/Database.hpp>

// C++ includes
#include <span>

auto Orm::fetchAccountPassword(std::string_view const accountName) -> std::optional<std::vector<byte>>
{
    std::optional<std::vector<byte>> result;

    SQLite::Statement query(Database::instance(), "SELECT password FROM accounts WHERE name LIKE ?");
    query.bindNoCopy(1, accountName.data());
    if (query.executeStep())
    {
        auto const & col = query.getColumn("password");
        result.emplace().append_range(std::span(static_cast<byte const *>(col.getBlob()), col.getBytes()));
    }
    return result;
}

void Orm::createAccount(std::string_view accountName, std::string_view password)
{
    SQLite::Statement query(Database::instance(), "INSERT INTO accounts (name, password) VALUES (?, ?)");
    query.bindNoCopy(1, accountName.data());
    query.bindNoCopy(2, password.data());
    query.exec();
}
