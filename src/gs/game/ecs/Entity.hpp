/// @author    Chnossos
/// @date      Created on 2026-03-15

#pragma once

// Project includes
#include <common/OptRef.hpp>
#include <common/core/Exception.hpp>
#include <gs/game/ecs/Component.hpp>

// C++ includes
#include <memory>
#include <typeindex>
#include <unordered_map>

class Entity
{
public:
    Entity() = default;
    Entity(Entity const &) = delete;
    Entity & operator=(Entity const &) = delete;
    Entity(Entity &&) noexcept = default;
    Entity & operator=(Entity &&) noexcept = default;
    virtual ~Entity() = default;

public:
    template<typename T> requires std::is_base_of_v<Component, T>
    auto component() -> OptRef<T>
    {
        auto const it = _components.find(typeid(T));
        return it != _components.end() ? OptRef<T>(static_cast<T &>(*it->second)) : std::nullopt;
    }

    template<typename T> requires std::is_base_of_v<Component, T>
    auto component() const -> OptRef<T const>
    {
        auto const it = _components.find(typeid(T));
        return it != _components.cend() ? OptRef<T const>(static_cast<T const &>(*it->second)) : std::nullopt;
    }

public:
    template<typename T, typename... Args> requires std::is_base_of_v<Component, T>
    auto addComponent(Args &&... args) -> T &
    {
        auto const & [it, ok] = _components.try_emplace(typeid(T), std::make_unique<T>(std::forward<Args>(args)...));
        L2CPP_B_ASSERT(ok, "Failed to insert component '{}' into entity", typeid(T).name());
        return static_cast<T &>(*it->second);
    }

    template<typename T, typename... Args> requires std::is_base_of_v<Component, T>
    auto getOrAddComponent(Args &&... args) -> T &
    {
        auto it = _components.find(typeid(T));
        if (it == _components.end())
            it  = _components.try_emplace(typeid(T), std::make_unique<T>(std::forward<Args>(args)...)).first;

        return static_cast<T &>(*it->second);
    }

    template<typename T> requires std::is_base_of_v<Component, T>
    auto delComponent() -> std::unique_ptr<T>
    {
        std::unique_ptr<T> component;

        if (auto const it = _components.find(typeid(T)); it != _components.end())
        {
            component.reset(static_cast<T *>(it->second.release()));
            _components.erase(it);
        }

        return component;
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> _components;
};
