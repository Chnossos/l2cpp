/// @author    Chnossos
/// @date      Created on 2026-06-02

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/constants/Profession.hpp>

// C++ includes
#include <array>
#include <unordered_map>

class Character;

namespace Orm
{
    void loadCharacterTemplates();
}

class CharacterTemplateDirectory
{
    friend void Orm::loadCharacterTemplates();

    struct CharacterTemplate
    {
        std::array<double, 2> collisionHeight, collisionRadius;
        u16 pAtk,      mAtk;
        u16 pDef,      mDef;
        u16 pAtkSpeed, mAtkSpeed;
        u16 runSpeed,  walkSpeed;
        u8  STR, DEX, CON, INT, WIT, MEN;

        void apply(Character &) const;
    };

public:
    CharacterTemplateDirectory() noexcept = delete;

public:
    static auto count()          -> size_t;
    static auto find(Profession) -> OptRef<CharacterTemplate const>;

public:
    static void load();

private:
    static std::unordered_map<u32, CharacterTemplate> _templates;
};
