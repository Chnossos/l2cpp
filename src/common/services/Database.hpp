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

namespace Utils::Sql
{
    template<typename T> requires std::integral<T> || std::floating_point<T>
    void extract(SQLite::Statement const & query, char const * columnName, T & value) {
        value = static_cast<T>(query.getColumn(columnName).getInt64());
    }

    template<typename T, typename U> requires std::integral<T> && std::integral<U>
    void extractOr(SQLite::Statement const & query, char const * columnName, T & value, U const elseValue)
    {
        auto const col = query.getColumn(columnName);
        value = static_cast<T>(!col.isNull() ? col.getInt64() : elseValue);
    }

    inline void extract(SQLite::Statement const & query, char const * columnName, std::string & value) {
        value = query.getColumn(columnName).getString();
    }

    inline void extractOr(SQLite::Statement const & query, char const * columnName,
                          std::string & value, std::string elseValue)
    {
        auto const col = query.getColumn(columnName);
        value = !col.isNull() ? col.getString() : std::move(elseValue);
    }

    template<typename Duration, typename Rep, typename Period>
    void extract(SQLite::Statement const & query, char const * columnName, std::chrono::duration<Rep, Period> & value) {
        value = Duration{1} * query.getColumn(columnName).getInt64();
    }

    template<typename Duration, typename Rep, typename Period>
    void extractOr(SQLite::Statement const & query, char const * columnName,
                   std::chrono::duration<Rep, Period> & value, Duration const elseValue)
    {
        auto const col = query.getColumn(columnName);
        value = !col.isNull() ? Duration{1} * col.getInt64() : elseValue;
    }
}
