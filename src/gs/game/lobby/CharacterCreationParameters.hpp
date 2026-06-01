/// @author    Chnossos
/// @date      Created on 2026-05-06

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../constants/Profession.hpp"
#include "../constants/Race.hpp"
#include "../constants/Sex.hpp"

struct CharacterCreationParameters
{
    std::wstring name;
    Race race;
    Sex sex;
    Profession profession;
    u32 INT, STR, CON, MEN, DEX, WIT, hairStyle, hairColor, face;
};
