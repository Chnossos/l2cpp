/// @author    Chnossos
/// @date      Created on 2026-03-23

// ReSharper disable CppNonExplicitConversionOperator
// ReSharper disable CppNonExplicitConvertingConstructor

#pragma once

// C++ includes
#include <optional>

/// My own take of @code std::optional<std::reference_wrapper<T>> @endcode to avoid writing @c opt->get() everywhere.
template<typename T>
class OptRef
{
    static_assert(!std::is_reference_v<T>,            "T cannot be a reference to a reference");
    static_assert(!std::is_same_v<T, std::nullopt_t>, "T cannot be std::nullopt_t");

public:
    constexpr OptRef() noexcept: _ref(nullptr) {}
    constexpr OptRef(std::nullopt_t) noexcept: OptRef() {}
    constexpr OptRef(T & t) noexcept: _ref(&t) {}

    constexpr OptRef(OptRef const & other) noexcept: _ref(other._ref) {}
    constexpr OptRef & operator=(OptRef const & other) noexcept { return assign(other._ref); }

    constexpr OptRef(OptRef && other) noexcept : _ref(std::exchange(other._ref, nullptr)) {}
    constexpr OptRef & operator=(OptRef && other) noexcept { return assign(std::exchange(other._ref, nullptr)); }

    template<typename U> requires std::is_base_of_v<T, U>
    constexpr OptRef(OptRef<U> const & other) noexcept: _ref(other ? &*other : nullptr) {}

public:
    constexpr void reset()                       { _ref = nullptr;         }
    constexpr OptRef & operator=(std::nullopt_t) { return assign(nullptr); }
    constexpr OptRef & operator=(T & t)          { return assign(&t);      }

public:
    constexpr bool has_value()         const { return _ref != nullptr; }
    constexpr explicit operator bool() const { return _ref != nullptr; }

public:
    // The following return non-const T because that's the behavior of std::reference_wrapper

    constexpr T & operator*()  const { return *_ref; }
    constexpr T * operator->() const { return  _ref; }
    constexpr operator T &()   const { return *_ref; }

    constexpr auto value() const -> T & { return *_ref; }

private:
    constexpr OptRef & assign(T * t)
    {
        _ref = t;
        return *this;
    }

private:
    T * _ref;
};

template <typename T>
OptRef(T) -> OptRef<T>;
