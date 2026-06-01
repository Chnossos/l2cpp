/// @author    Chnossos
/// @date      Created on 2026-05-06

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/constants/Profession.hpp>
#include <gs/game/constants/Race.hpp>
#include <gs/game/constants/Sex.hpp>

struct CharacterCreationParameters
{
    std::wstring name;
    Race         race;
    Sex          sex;
    Profession   profession;
    u32          INT, STR, CON, MEN, DEX, WIT, hairStyle, hairColor, face;
};
