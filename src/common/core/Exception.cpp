/// @author    Chnossos
/// @date      Created on 20/07/2021

#include "Exception.hpp"

namespace
{
    auto fileName(std::string_view path) -> std::string_view
    {
#if defined(_WIN32) && !defined(__CYGWIN__)
        constexpr char separator = '\\';
#else
        constexpr char separator = '/';
#endif

        auto const pos = path.find_last_of(separator);
        return pos != std::string::npos ? path.substr(pos + 1) : path;
    }

    void getExceptionStack(Core::Exception const & e, Core::ExceptionStack & stack)
    {
        stack.emplace_back(e);
        try
        {
            std::rethrow_if_nested(e);
        }
        catch (Core::Exception const & nested)
        {
            getExceptionStack(nested, stack);
        }
        catch (std::exception const & nested)
        {
            getExceptionStack(Core::Exception({}, 0, "{}",  nested.what()), stack);
        }
    }

    std::string formatSourceLocation(Core::Exception const & e)
    {
        std::string src;

        if (!e.fileBaseName().empty())
            src += e.fileBaseName();

        if (e.line() > 0)
        {
            if (!src.empty())
                src += ':';

            src += std::format("{}", e.line());
        }

        if (!src.empty())
            src = std::format(" [{}]", src);

        return src;
    }
} // !namespace

Core::Exception::Exception() noexcept
    : Exception(0)
{}

Core::Exception::Exception(int const code) noexcept
    : Exception({}, code)
{}

Core::Exception::Exception(std::source_location const & src, int const code) noexcept
    : _source(src)
    , _code(code)
{}

auto Core::Exception::code() const -> int      { return _code;          }
auto Core::Exception::line() const -> unsigned { return _source.line(); }

auto Core::Exception::fileBaseName() const  -> std::string_view
{
    return fileName(_source.file_name());
}

auto Core::Exception::functionName() const  -> std::string_view { return _source.function_name(); }
auto Core::Exception::what() const noexcept -> char const *     { return _message.c_str();        }

void Core::Exception::init(std::source_location const & src, std::string message)
{
    init(src, 0, std::move(message));
}

void Core::Exception::init(std::source_location const & src, int const code)
{
    init(src, code, std::string());
}

void Core::Exception::init(std::source_location const & src, int const code, std::string message)
{
    _source  = src;
    _message = std::move(message);
    _code    = code;
}

auto Core::sandBox(std::function<void()> const & f, std::source_location const & src) -> ExceptionBox
{
    ExceptionBox box { nullptr, src };
    try         { f();                                }
    catch (...) { box.ptr = std::current_exception(); }
    return box;
}

auto Core::getExceptionStack(Exception const & e) -> ExceptionStack
{
    ExceptionStack stack;
    ::getExceptionStack(e, stack);
    return stack;
}

auto Core::formatExceptionStack(std::exception const & e, int level, int increment,
                                 std::string_view prefix, std::string_view suffix)
    -> std::string
{
    ExceptionStack stack;
    stack.push_back(Exception("{}",  e.what())); // NOLINT(*-use-emplace)
    try
    {
        std::rethrow_if_nested(e);
    }
    catch (Exception const & nested)
    {
        ::getExceptionStack(nested, stack);
    }
    catch (std::exception const & nested)
    {
        ::getExceptionStack(Exception("{}",  nested.what()), stack);
    }
    return formatExceptionStack(stack, level, increment, prefix, suffix);
}

auto Core::formatExceptionStack(Exception const & e, int level, int increment,
                                 std::string_view prefix, std::string_view suffix)
    -> std::string
{
    return formatExceptionStack(getExceptionStack(e), level, increment, prefix, suffix);
}

auto Core::formatExceptionStack(ExceptionStack const & s, int level, int increment,
                                 std::string_view prefix, std::string_view suffix)
    -> std::string
{
    std::string str;
    for (auto const & e : s)
    {
        auto const code = e.code() ? std::format(" (code: {})", e.code()) : "";
        auto const src  = formatSourceLocation(e);
        str += std::format("{:>{}}{}{}{}{}",
                           prefix, static_cast<int>(prefix.size()) + level, e.what(), code, src, suffix);
        level += increment;
    }
    return str;
}
