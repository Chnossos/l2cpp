/// @author    Chnossos
/// @date      Created on 2026-05-05

#include "Database.hpp"

// Project includes
#include <common/core/Exception.hpp>
#include <common/utils/File.hpp>

void Database::init(std::vector<std::filesystem::path> const & files)
{
    auto & db = instance();

    L2CPP_F_ASSERT([&db] { db.exec("PRAGMA foreign_keys = ON"); }, "Failed to enable foreign keys check");

    for (auto const & path : files)
        db.exec(Utils::readWholeTextFile(path));
}

auto Database::instance() -> SQLite::Database &
{
    static SQLite::Database db("data/database", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    return db;
}
