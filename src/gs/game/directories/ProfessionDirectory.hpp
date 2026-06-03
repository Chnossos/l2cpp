/// @author    Chnossos
/// @date      Created on 2026-06-03

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/constants/Profession.hpp>

namespace Orm
{
    void loadProfessions();
}

class ProfessionDirectory
{
    friend void Orm::loadProfessions();

    ProfessionDirectory() = delete;

// public:
    struct ProfessionInfo
    {
        std::string name;
        float       maxHp, hpFlatPerLevel, hpMultiplierPerLevel;
        float       maxMp, mpFlatPerLevel, mpMultiplierPerLevel;
        float       maxCp, cpFlatPerLevel, cpMultiplierPerLevel;
        Profession  profession, parentProfession;
        u16         pAtk,      mAtk;
        u16         pDef,      mDef;
        u16         pAtkSpeed, mAtkSpeed;
        u16         runSpeed,  walkSpeed;
        u8          STR, DEX, CON, INT, WIT, MEN;
        bool        canBeSubclassed;
    };

public:
    static auto count() -> size_t;
    static auto find(Profession) -> OptRef<ProfessionInfo const>;

public:
    static void load();

private:
    static std::unordered_map<u32, ProfessionInfo> _professions;
};
