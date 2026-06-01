/// @author    Chnossos
/// @date      Created on 2026-03-16

#pragma once

// Project includes
#include "../constants/Profession.hpp"
#include "../constants/Race.hpp"
#include "../constants/Sex.hpp"
#include "ActorAppearance.hpp"

class PlayerAppearance : public ActorAppearance
{
public:
    PlayerAppearance() noexcept;

public:
    auto race()               const -> Race;
    auto startingProfession() const -> Profession;
    auto sex()                const -> Sex;
    auto hairStyle()          const -> u32;
    auto hairColor()          const -> u32;
    auto face()               const -> u32;
    auto nameColor()          const -> u32;

public:
    void setStartingProfession(Profession);
    void setSex(Sex);
    void setHairStyle(u32);
    void setHairColor(u32);
    void setFace(u32);
    void setNameColor(u32);

private:
    Race       _race;
    Profession _startingProfession;
    Sex        _sex;
    u32        _hairStyle;
    u32        _hairColor;
    u32        _face;
    u32        _nameColor;
};
