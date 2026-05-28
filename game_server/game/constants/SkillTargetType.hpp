/// @author    Chnossos
/// @date      Created on 2026-03-11

#pragma once

enum class SkillTargetType
{
    /// Skill effects automatically apply to self only, no target needed.
    /// <b>Examples:</b> <i>War Cry</i>, <i>Self Heal</i>…
    Self,

    /// Skill effects apply to the target only.
    /// <b>Examples:</b> <i>Wind Strike</i>, <i>Resurrection</i>…
    Single,

    /// Skill effects apply to and around the target.
    /// <b>Examples:</b> <i>Aqua Splash</i>, <i>Mass Resurrection</i>…
    Multiple,

    /// Skill effects apply to and around self, no target needed.
    /// <b>Examples:</b> <i>Group Heal</i>, <i>Chant of Life</i>…
    Aura,
};
