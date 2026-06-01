/// @author    Chnossos
/// @date      Created on 2026-03-14

#pragma once

// Project includes
#include <common/Pimpl.hpp>
#include <gs/game/ecs/Component.hpp>
#include <gs/game/skill/Skill.hpp>

/// List of skills a (N)PC has learned.
class SkillDirectory : public Component
{
public:
    SkillDirectory();
    SkillDirectory(SkillDirectory &&) noexcept;
    SkillDirectory & operator=(SkillDirectory &&) noexcept;
    ~SkillDirectory() override;

public:
    auto size() const -> size_t;
    auto skill(SkillId id) -> OptRef<Skill>;
    auto skill(SkillId id) const -> OptRef<Skill const>;

public:
    auto learn(SkillId id, SkillLevel level) -> OptRef<Skill>;

public:
    auto begin() -> std::unordered_map<SkillId, Skill>::iterator;
    auto end()   -> std::unordered_map<SkillId, Skill>::iterator;

    auto begin() const -> std::unordered_map<SkillId, Skill>::const_iterator;
    auto end()   const -> std::unordered_map<SkillId, Skill>::const_iterator;

    auto cbegin() const -> std::unordered_map<SkillId, Skill>::const_iterator;
    auto cend()   const -> std::unordered_map<SkillId, Skill>::const_iterator;

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
