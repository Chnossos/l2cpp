/// @author    Chnossos
/// @date      Created on 2026-03-05

#include "GameObject.hpp"

// Project includes
#include <l2cpp/Exception.hpp>
#include <l2cpp/details/Pimpl.hpp>

struct GameObject::Impl
{
    GameObjectId id = 0;
};

template class Pimpl<GameObject::Impl>;

GameObject::GameObject()
{
    // naive approach: allows for UINT32_MAX ids during a single server run
    static GameObjectId id = 1; // client doesn't accept 0

    L2CPP_B_ASSERT(id < std::numeric_limits<GameObjectId>::max(),
                   "Failed to create GameObject: no more spare id to give");

    _impl->id = id++;
}

GameObject::GameObject(GameObject && other)       noexcept = default;
GameObject & GameObject::operator=(GameObject &&) noexcept = default;
GameObject::~GameObject()                                  = default;

auto GameObject::id() const -> GameObjectId
{
    return _impl->id;
}
