/// @author    Chnossos
/// @date      Created on 2026-05-06

#pragma once

enum class CharacterCreationResult
{
    Success = -1,

    Failure,
    TooManyCharacters,
    NameAlreadyTaken,
    InvalidName,
};
