/// @author    Chnossos
/// @date      Created on 2026-03-16

#pragma once

// Project includes
#include <gs/game/ecs/Component.hpp>

class ActorAppearance : public Component
{
public:
    ~ActorAppearance() override = 0;

public:
    auto collisionHeight() const -> double { return _collisionHeight; }
    auto collisionRadius() const -> double { return _collisionRadius; }

public:
    void setCollisionHeight(double const height) { _collisionHeight = height; }
    void setCollisionRadius(double const radius) { _collisionRadius = radius; }

private:
    double _collisionHeight = 0;
    double _collisionRadius = 0;
};

inline ActorAppearance::~ActorAppearance() = default;
