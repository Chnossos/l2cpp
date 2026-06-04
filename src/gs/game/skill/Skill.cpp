/// @author    Chnossos
/// @date      Created on 2026-03-11

#include "Skill.hpp"

// Project includes
#include <common/core/Exception.hpp>
// ReSharper disable once CppUnusedIncludeDirective
#include <common/details/Pimpl.hpp>
#include <gs/game/directories/SkillTemplateDirectory.hpp>

// C++ includes
#include <chrono>

using namespace std::chrono;

struct Skill::Impl
{
    Ref<SkillTemplate const> tmplate;
    system_clock::time_point lastUse{};
};

template class Pimpl<Skill::Impl>;

// ---------------------------------------------------------------------------------------------------------------------

Skill::Skill(SkillTemplate const & skillTemplate)
    : _impl(skillTemplate)
{}

Skill::Skill(Skill &&) noexcept = default;
Skill & Skill::operator=(Skill &&) noexcept = default;
Skill::~Skill() = default;

auto Skill::tmplate() const -> SkillTemplate const &
{
    return _impl->tmplate;
}

void Skill::setLevel(SkillLevel const level)
{
    auto const & tmpl = _impl->tmplate.get();
    auto const skill  = SkillTemplateDirectory::skill(tmpl.id(), level);
    L2CPP_B_ASSERT(skill, "Failed to change skill '{}' level from '{}' to '{}'", tmpl.id(), tmpl.level(), level);
    _impl->tmplate = *skill;
}
