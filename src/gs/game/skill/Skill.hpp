/// @author    Chnossos
/// @date      Created on 2026-03-11

#pragma once

// Project includes
#include "SkillTemplate.hpp"

#include <l2cpp/Pimpl.hpp>

// C++ includes
#include <unordered_map>

class Skill
{
public:
    explicit Skill(SkillTemplate const & skillTemplate);
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
