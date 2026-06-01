/// @author    Chnossos
/// @date      Created on 2026-02-18

#pragma once

// C++ includes
#include <memory>

template<typename T>
class Pimpl
{
public:
    template<typename... Args>
    explicit Pimpl(Args &&...);
    ~Pimpl();

public:
    Pimpl(Pimpl const &)             = delete;
    Pimpl & operator=(Pimpl const &) = delete;

public:
    Pimpl(Pimpl && other) noexcept;
    Pimpl & operator=(Pimpl && other) noexcept;

public:
    auto operator->()       -> T *      ;
    auto operator->() const -> T const *;

    auto operator*()       -> T &      ;
    auto operator*() const -> T const &;

private:
    std::unique_ptr<T> _impl;
};
