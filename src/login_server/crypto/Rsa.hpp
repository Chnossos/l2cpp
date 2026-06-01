/// @author    Chnossos
/// @date      Created on 2026-04-11

#pragma once

// Project includes
#include <l2cpp/Pimpl.hpp>
#include <l2cpp/Typedefs.hpp>

// C++ includes
#include <span>
#include <string_view>

class Rsa
{
    explicit Rsa(std::string_view exponent);
    ~Rsa();

public:
    static auto instance() -> Rsa &;

public:
    auto modulus() const -> std::span<byte const>;

public:
    void decrypt(std::span<byte> buffer);

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
