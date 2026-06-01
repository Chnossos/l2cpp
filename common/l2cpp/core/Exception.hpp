/// @author    Chnossos
/// @date      Created on 06/01/2021

#pragma once

#include <exception>
#include <format>
#include <functional>
#include <print>
#include <source_location>
#include <string_view>
#include <vector>

namespace Core
{
    /// Extends @c std::runtime_error with string formatting and an error code
    class Exception : public std::exception
    {
    private:
        std::source_location _source;
        std::string          _message;
        int                  _code; ///< Zero means uninitialized

    public:
        Exception() noexcept;

        explicit Exception(int code) noexcept;
        Exception(std::source_location src, int code) noexcept;

        template<typename... Args>
        explicit Exception(std::format_string<Args...> fmt, Args &&... args)
        {
            init(std::source_location(), std::format(std::move(fmt), std::forward<Args>(args)...));
        }

        template<typename... Args>
        Exception(std::source_location src, std::format_string<Args...> fmt, Args &&... args)
        {
            init(std::move(src), std::format(std::move(fmt), std::forward<Args>(args)...));
        }

        template<typename... Args>
        Exception(int const code, std::format_string<Args...> fmt, Args &&...args)
        {
            init(std::source_location(), code, std::format(std::move(fmt), std::forward<Args>(args)...));
        }

        template<typename... Args>
        Exception(std::source_location src, int const code, std::format_string<Args...> fmt, Args &&...args)
        {
            init(std::move(src), code, std::format(std::move(fmt), std::forward<Args>(args)...));
        }

    public:
        /// Gets the error code associated with this exception, if given
        /// @return 0 if uninitialized, else a non-zero value
        auto code() const -> int;

        /// Gets the source code line number that generated the exception, if given
        /// @return 0 if location is unknown, else a non-zero value
        auto line() const -> int;

        /// Gets the source code file base name that generated the exception, if given
        /// @return Empty string if location is unknown, else a non-zero value
        auto fileBaseName() const -> std::string_view;

        /// Gets the function name that generated the exception, if given
        /// @warning Different compilers will wield different naming convention, do not rely on this
        ///          string being the same, even between two runs of the same program!
        /// @returns Empty string if function name is unknown, else a non-zero value
        auto functionName() const -> std::string_view;

        auto what() const noexcept -> char const * override;

    private:
        void init(std::source_location src, std::string message);
        void init(std::source_location src, int code);
        void init(std::source_location src, int code, std::string message);
    };

    /// Packs a @c std::exception_ptr with a @c std::source_location to locate its origin
    struct ExceptionBox
    {
        std::exception_ptr        ptr;
        std::source_location sourceLocation;

        explicit operator bool() const { return ptr.operator bool(); }
    };

    using ExceptionStack = std::vector<Exception>;

    auto sandBox(std::source_location, std::function<void()>) -> ExceptionBox;

    template<typename F, typename... Args>
    auto sandBox(std::source_location src, F && f, Args &&... args) -> ExceptionBox
    {
        std::function<void()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        return sandBox(std::move(src), std::move(func));
    }

    auto getExceptionStack(Exception const & e) -> ExceptionStack;

    auto formatExceptionStack(std::exception const & e, int level = 0, int increment = 1,
                             std::string_view prefix = " * ", std::string_view suffix = "\n") -> std::string;

    auto formatExceptionStack(Exception const & e, int level = 0, int increment = 1,
                             std::string_view prefix = " * ", std::string_view suffix = "\n") -> std::string;

    auto formatExceptionStack(ExceptionStack const & s, int level = 0, int increment = 1,
                             std::string_view prefix = " * ", std::string_view suffix = "\n") -> std::string;

    template<typename E, typename... Args>
    void printExceptionStack(E const & e, Args &&... args)
    {
        std::print("{}", formatExceptionStack(e, std::forward<Args>(args)...));
    }
}

#define L2CPP_SOURCE_LOCATION std::source_location::current()

#define L2CPP_EXCEPTION(...) Core::Exception(L2CPP_SOURCE_LOCATION, __VA_ARGS__)

/// Same as a regular throw of a @c Core::Exception but adds source location info to the call
#define L2CPP_THROW(...) throw L2CPP_EXCEPTION(__VA_ARGS__)

/// Rethrows the current exception but nested under a new @c Core::Exception formatted
/// with format and variadic arguments
#define L2CPP_THROW_NESTED(...) std::throw_with_nested(L2CPP_EXCEPTION(__VA_ARGS__))

/// Boolean assertion. Throws a @c Core::Exception formatted with format and variadic arguments
/// if @p expr is @c false
#define L2CPP_B_ASSERT(expr, ...) do { if (!(expr)) L2CPP_THROW(__VA_ARGS__); } while (0)

/// Status code assertion. Throws a @c Core::Exception initialized with @p code and formatted
/// with format and variadic arguments if @p code is not 0
#define L2CPP_C_ASSERT(code, ...) L2CPP_B_ASSERT((code) == 0, code, __VA_ARGS__)

/// Boolean assertion. Throws a @c Core::Exception formatted with format and variadic arguments
/// if @p expr is @c false
#define L2CPP_BC_ASSERT(expr, code, ...) L2CPP_B_ASSERT(expr, code, __VA_ARGS__)

/// Status code assertion. Throws a @c Core::Exception initialized with @p code and formatted
/// with format and variadic arguments if @p exitCode is not 0
#define L2CPP_CC_ASSERT(exitCode, code, ...) L2CPP_B_ASSERT((exitCode) == 0, code, __VA_ARGS__)

/// Catches any exception thrown by @p callable and rethrows it but nested under a new exception as
/// a formatted message with format and variadic arguments
#define L2CPP_F_ASSERT(callable, ...) \
    do try { (callable)(); } catch (...) { L2CPP_THROW_NESTED(__VA_ARGS__); } while (0)

/// Rethrows @p exceptionPtr if not null, nested under a new exception formatted with format and
/// variadic arguments
#define L2CPP_E_ASSERT(exceptionPtr, ...) \
    L2CPP_F_ASSERT([&] { if (exceptionPtr) std::rethrow_exception(exceptionPtr); }, __VA_ARGS__)

#define L2CPP_SANDBOX(callable) Core::sandBox(L2CPP_SOURCE_LOCATION, callable)
