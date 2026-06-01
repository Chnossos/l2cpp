/// @author    Chnossos
/// @date      Created on 2026-04-28

#pragma once

#ifndef fire
# define fire
#endif

// C++ includes
#include <functional>

template<typename... Args>
class Event
{
    using Callback = std::function<void(Args...)>;

public:
    auto operator+=(Callback && function) -> Event &
    {
        if (function)
            _functions.emplace_back(std::move(function));

        return *this;
    }

    void operator()(Args &&... args)
    {
        for (auto const & f : _functions)
            f(std::forward<Args>(args)...);
    }

    void clear() { _functions.clear(); }

private:
    std::list<std::function<void(Args...)>> _functions;
};

template<typename... Args>
Event(Args &&...) -> Event<Args...>;
