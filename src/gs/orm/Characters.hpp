/// @author    Chnossos
/// @date      Created on 2026-05-09

#pragma once

class Character;

namespace Orm
{
    void loadProfessions();
    void loadCharacterTemplates();

    void saveCharacter(Character const &);
    void loadCharacter(Character &);
}
