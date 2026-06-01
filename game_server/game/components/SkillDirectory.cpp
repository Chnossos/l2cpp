/// @author    Chnossos
/// @date      Created on 2026-03-14

#include "SkillDirectory.hpp"

// Project includes
#include "../skill/SkillTemplateDirectory.hpp"

#include <l2cpp/Exception.hpp>
#include <l2cpp/details/Pimpl.hpp>

// Third-party includes
#include <spdlog/spdlog.h>

// C++ includes
#include <unordered_map>

struct SkillDirectory::Impl
{
    std::unordered_map<SkillId, Skill> skills;
};

template class Pimpl<SkillDirectory::Impl>;

// ---------------------------------------------------------------------------------------------------------------------

SkillDirectory::SkillDirectory() = default;
SkillDirectory::SkillDirectory(SkillDirectory &&) noexcept = default;
SkillDirectory & SkillDirectory::operator=(SkillDirectory &&) noexcept = default;
SkillDirectory::~SkillDirectory() = default;

auto SkillDirectory::size() const -> size_t
{
    return _impl->skills.size();
}

auto SkillDirectory::skill(SkillId const id) -> OptRef<Skill>
{
    auto const it = _impl->skills.find(id);
    return it != _impl->skills.end() ? OptRef(it->second) : std::nullopt;
}

auto SkillDirectory::skill(SkillId const id) const -> OptRef<Skill const>
{
    return *const_cast<SkillDirectory *>(this)->skill(id);
}

auto SkillDirectory::learn(SkillId const id, SkillLevel const level) -> OptRef<Skill>
{
    OptRef<Skill> s;

    if (s = skill(id); s)
        s->setLevel(level);
    else if (auto const tmpl = SkillTemplateDirectory::skill(id, level))
        s = _impl->skills.try_emplace(id, *tmpl).first->second;
    else
        SPDLOG_WARN("Failed to learn skill '{}' at level '{}': not found", id, level);

    return s;
}

auto SkillDirectory::begin()        -> std::unordered_map<SkillId, Skill>::iterator       { return _impl->skills.begin();  }
auto SkillDirectory::end()          -> std::unordered_map<SkillId, Skill>::iterator       { return _impl->skills.end();    }
auto SkillDirectory::begin()  const -> std::unordered_map<SkillId, Skill>::const_iterator { return _impl->skills.cbegin(); }
auto SkillDirectory::end()    const -> std::unordered_map<SkillId, Skill>::const_iterator { return _impl->skills.cend();   }
auto SkillDirectory::cbegin() const -> std::unordered_map<SkillId, Skill>::const_iterator { return _impl->skills.cbegin(); }
auto SkillDirectory::cend()   const -> std::unordered_map<SkillId, Skill>::const_iterator { return _impl->skills.cend();   }
