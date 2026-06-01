/// @author    Chnossos
/// @date      Created on 2026-03-13

#pragma once

// Project includes
#include "SkillTemplate.hpp"

// C++ includes/
#include <filesystem>
#include <unordered_map>

class SkillTemplateDirectory
{
private:
    SkillTemplateDirectory() noexcept = default;

public:
    static void load(std::filesystem::path const & skillNamesFile, std::filesystem::path const & skillGroupsFile);

public:
    static auto size() -> size_t;
    static auto skill(SkillUid uid) -> OptRef<SkillTemplate>;
    static auto skill(SkillId const id, SkillLevel const lvl) { return skill({id, lvl}); }

private:
    static std::unordered_map<SkillUid, SkillTemplate> _templates;
};
