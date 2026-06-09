/// @author    Chnossos
/// @date      Created on 2026-06-09

#pragma once

// Project includes
#include <gs/game/ecs/Component.hpp>
#include <gs/Typedefs.hpp>

// C++ includes
#include <unordered_set>

struct KnownActors : public Component
{
    std::unordered_set<GameObjectId> ids;
};
