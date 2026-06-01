/// @author    Chnossos
/// @date      Created on 2026-03-15

#pragma once

// Project includes
#include <gs/game/ecs/Component.hpp>

// C++ includes
#include <string>

struct ActorIdentity : public Component
{
    std::wstring name, title;

    explicit ActorIdentity(std::wstring name_ = {}, std::wstring title_ = {}) noexcept
        : name(std::move(name_)), title(std::move(title_))
    {}
};
