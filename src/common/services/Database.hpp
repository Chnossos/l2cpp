/// @author    Chnossos
/// @date      Created on 2026-05-05

// ReSharper disable CppUnusedIncludeDirective

#pragma once

// Third-party includes
#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Transaction.h>

// C++ includes
#include <filesystem>

class Database
{
public:
    static void init(std::vector<std::filesystem::path> const & files);
    static auto instance() -> SQLite::Database &;
};
