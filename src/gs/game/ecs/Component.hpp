/// @author    Chnossos
/// @date      Created on 2026-03-15

#pragma once

struct Component
{
    Component() noexcept = default;
    Component(Component const &) noexcept = default;
    Component & operator=(Component const &) noexcept = default;
    Component(Component &&) noexcept = default;
    Component & operator=(Component &&) noexcept = default;
    virtual ~Component() noexcept = 0;
};

inline Component::~Component() noexcept = default;
