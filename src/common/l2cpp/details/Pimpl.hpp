/// @author    Chnossos
/// @date      Created on 2026-03-03

#pragma once

// C++ includes
#include <utility>

template<typename T>
template<typename... Args>
Pimpl<T>::Pimpl(Args &&... args): _impl(new T{std::forward<Args>(args)...}) {}

template<typename T>
Pimpl<T>::~Pimpl() = default;

template<typename T>
Pimpl<T>::Pimpl(Pimpl && other) noexcept: _impl(std::move(other._impl)) {}

template<typename T>
Pimpl<T> & Pimpl<T>::operator=(Pimpl && other) noexcept
{
    _impl = std::move(other._impl);
    return *this;
}

template<typename T> auto Pimpl<T>::operator->() -> T * { return  _impl.get(); }
template<typename T> auto Pimpl<T>::operator*()  -> T & { return *_impl.get(); }

template<typename T> auto Pimpl<T>::operator->() const -> T const * { return  _impl.get(); }
template<typename T> auto Pimpl<T>::operator*()  const -> T const & { return *_impl.get(); }
