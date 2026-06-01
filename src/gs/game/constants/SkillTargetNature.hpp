/// @author    Chnossos
/// @date      Created on 2026-04-21

#pragma once

enum class SkillTargetNature
{
    None        = 0b0000000000000000, ///< Skill is animation only
    Self        = 0b0000000000000001, ///< Target can be self
    Enemy       = 0b0000000000000010, ///< Target can be an enemy (opponent, clan war, monster…)
    Friendly    = 0b0000000000000100, ///< Target can be friendly (same team, party, clan…)
    Corpse      = 0b0000000000001000, ///< Target can be a corpse
    Character   = 0b0000000000010000, ///< Target can be a character
    Monster     = 0b0000000000100000, ///< Target can be a monster
    Npc         = 0b0000000001000000, ///< Target can be a npc
    Party       = 0b0000000010000000, ///< Target can be a party member
    Clan        = 0b0000000100000000, ///< Target can be a clan member
    Alliance    = 0b0000001000000000, ///< Target can be an alliance member
};
