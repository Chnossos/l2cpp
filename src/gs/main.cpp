/// @author    Chnossos
/// @date      Created on 2026-02-17

// Project includes
#include <common/core/Exception.hpp>
#include <gs/Application.hpp>

// Third-party
#include <spdlog/spdlog.h>

static void setupLogger();
static auto getCurrentLocale() -> std::string;

int main(int const argc, char const * const argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    setupLogger();
    SPDLOG_INFO("Starting {} version {}", L2CPP_SUBPROJECT_NAME, L2CPP_VERSION);

    {
        // std::locale const currentLocale(getCurrentLocale());
        std::locale const currentLocale("en-UK");
        std::locale::global(currentLocale);
        SPDLOG_TRACE("Current locale: {}", currentLocale.name().c_str());
    }

    int code = EXIT_FAILURE;
    try
    {
        Application app({argv, argv + argc});
        if (app.load())
            code = app.run();
    }
    catch (Core::Exception const & e)
    {
        SPDLOG_CRITICAL("Caught top-level exception:\n{}", Core::formatExceptionStack(e));
    }
    catch (std::exception const & e)
    {
        SPDLOG_CRITICAL("Caught top-level exception:\n{}", Core::formatExceptionStack(e));
    }
    catch (...)
    {
        SPDLOG_CRITICAL("Unexpected, non-std exception caught, terminating");
    }
    return code;
}

void setupLogger()
{
    spdlog::set_pattern("[%^%R:%S.%e%$] [%L] %v [%s:%#]");
    spdlog::set_level(spdlog::level::trace);
}

std::string getCurrentLocale()
{
    std::string result;

#ifdef _WIN32
    wchar_t name[LOCALE_NAME_MAX_LENGTH];
    if (auto size = LCIDToLocaleName(GetThreadLocale(), name, LOCALE_NAME_MAX_LENGTH, 0))
    {
        result.resize(static_cast<std::size_t>(--size)); // Decrement to remove leading '\0'
        // Locale names don't contain weird characters so it's okay
        std::transform(name, name + size, result.begin(), [] (wchar_t const c) { return static_cast<char>(c); });
    }
#endif

    return result;
}
