/// @author    Chnossos
/// @date      Created on 2026-03-03

#pragma once

// Project includes
#include "../Typedefs.hpp"

#include <common/Pimpl.hpp>

class GameObject
{
public:
    GameObject();
    GameObject(GameObject &&) noexcept;
    GameObject & operator=(GameObject &&) noexcept;
    virtual ~GameObject() = 0;

public:
    auto id() const -> GameObjectId;

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
