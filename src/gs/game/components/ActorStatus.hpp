/// @author    Chnossos
/// @date      Created on 2026-03-17

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/ecs/Component.hpp>

struct ActorStatus : public Component
{
    ActorStatus() noexcept = default;
    ActorStatus(ActorStatus const &) noexcept = default;
    ActorStatus & operator=(ActorStatus const &) noexcept = default;
    ActorStatus(ActorStatus &&) noexcept = default;
    ActorStatus & operator=(ActorStatus &&) noexcept = default;
    ~ActorStatus() override = 0;

    virtual auto level() const -> u32 = 0;

    virtual void setLevel(u32 level, double percent) = 0;
    void setLevel(u32 const level) { setLevel(level, 0); }
};

inline ActorStatus::~ActorStatus() = default;
