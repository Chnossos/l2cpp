/// @author    Chnossos
/// @date      Created on 2026-05-06

#pragma once

// Project includes
#include "../Typedefs.hpp"

class Character;

namespace Orm
{
    auto loadCharacterPreviews(AccountId) -> std::vector<Ref<Character>>;
    void createCharacter(AccountId, Character &);
    void selectCharacter(AccountId, std::wstring_view selectedCharName);
    void deleteCharacter(std::wstring_view charName);
}
