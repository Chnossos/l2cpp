/// @author    Chnossos
/// @date      Created on 2026-06-02

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/constants/Profession.hpp>
#include <gs/game/constants/Sex.hpp>

// C++ includes
#include <unordered_map>

namespace Orm
{
    void loadCharacterTemplates();
}

class CharacterTemplateDirectory
{
    friend void Orm::loadCharacterTemplates();

    CharacterTemplateDirectory() noexcept = delete;

public:
    static auto count() -> size_t;
    static auto collisionHeight(Profession startingProfession, Sex) -> double;
    static auto collisionRadius(Profession startingProfession, Sex) -> double;

public:
    static void load();

private:
    static auto makeId(Profession, Sex) -> u64;

private:
    static std::unordered_map<u64, PairOf<double>> _properties;
};
