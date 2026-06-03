/// @author    Chnossos
/// @date      Created on 2026-06-02

#include "CharacterTemplateDirectory.hpp"

// Project includes
#include <core/Exception.hpp>
#include <gs/orm/Characters.hpp>

auto CharacterTemplateDirectory::count() -> size_t
{
    return _properties.size();
}

auto CharacterTemplateDirectory::collisionHeight(Profession const startingProfession, Sex const sex) -> double
{
    auto const it = _properties.find(makeId(startingProfession, sex));
    L2CPP_B_ASSERT(it != _properties.end(),
                   "No collision height registered for profession/sex '{}'/'{}'",
                   std::to_underlying(startingProfession), std::to_underlying(sex));

    return it->second.first;
}

auto CharacterTemplateDirectory::collisionRadius(Profession const startingProfession, Sex const sex) -> double
{
    auto const it = _properties.find(makeId(startingProfession, sex));
    L2CPP_B_ASSERT(it != _properties.end(),
                   "No collision height registered for profession/sex '{}'/'{}'",
                   std::to_underlying(startingProfession), std::to_underlying(sex));

    return it->second.second;
}

void CharacterTemplateDirectory::load()
{
    Orm::loadCharacterTemplates();
}

auto CharacterTemplateDirectory::makeId(Profession const startingProfession, Sex const sex) -> u64
{
    return std::to_underlying(startingProfession) << 1 | std::to_underlying(sex);
}

std::unordered_map<u64, PairOf<double>> CharacterTemplateDirectory::_properties;
