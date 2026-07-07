/// @author    Chnossos
/// @date      Created on 2026-03-11

#pragma once

// Project includes
#include <common/Pimpl.hpp>
#include <gs/game/skill/SkillTemplate.hpp>

// C++ includes
#include <unordered_map>

class Skill
{
public:
    explicit Skill(SkillTemplate const & skillTemplate);
    Skill(Skill const &) noexcept = delete;
    Skill & operator=(Skill const &) noexcept = delete;
    Skill(Skill &&) noexcept;
    Skill & operator=(Skill &&) noexcept;
    ~Skill();

public:
    auto tmplate() const -> SkillTemplate const &;

public:
    void setLevel(SkillLevel level);

public:
    operator SkillTemplate const &() const { return tmplate(); }

private:
    struct Impl;
    Pimpl<Impl> _impl;
};

using SkillRegistry = std::unordered_map<SkillUid, Skill>;
