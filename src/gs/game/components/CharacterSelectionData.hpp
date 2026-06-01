/// @author    Chnossos
/// @date      Created on 2026-05-06

#pragma once

// Project includes
#include <gs/game/ecs/Component.hpp>

struct CharacterSelectionData : public Component
{
    bool selected = false;
};
