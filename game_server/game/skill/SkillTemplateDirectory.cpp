/// @author    Chnossos
/// @date      Created on 2026-03-13

#include "SkillTemplateDirectory.hpp"

// Project includes
#include "../../utils/Conversion.hpp"
#include "../effects/factories/DamageEffectFactory.hpp"
#include "../effects/factories/HealEffectFactory.hpp"
#include "../effects/factories/ResurrectionEffectFactory.hpp"
#include "../effects/factories/ToggleBuffEffectFactory.hpp"

#include <l2cpp/Exception.hpp>
#include <l2cpp/utils/EnumMask.hpp>

// Third-pary includes
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <spdlog/spdlog.h>

// C++ includes
#include <fstream>

std::unordered_map<SkillUid, SkillTemplate> SkillTemplateDirectory::_templates;

namespace
{
    void loadSkillNames(std::unordered_map<SkillUid, SkillTemplate> & infos, std::filesystem::path const & path)
    {
        constexpr auto minPartsCount = 5;
        constexpr auto maxPartsCount = 6;

        std::ifstream file(path);
        L2CPP_B_ASSERT(file, "Failed to open file '{}': {}", path.string(), std::strerror(errno));

        std::string line;
        std::vector<std::string_view> parts;
        parts.reserve(maxPartsCount); // skill_id, skill_level, skill_name, skill_desc, skill_ench_route_name, skill_ench_desc

        for (size_t i = 1; std::getline(file, line) && !line.empty(); ++i) try
        {
            boost::algorithm::split(parts, line, boost::algorithm::is_any_of("\t"));
            if (parts.size() < minPartsCount || parts.size() > maxPartsCount)
            {
                L2CPP_THROW("Found {} parts instead of minimum of {}:\nParts found: {::?}",
                           parts.size(), minPartsCount, parts);
            }

            SkillTemplate tmpl(Utils::stringViewTo<u16>(parts[0]), std::string(parts[2].substr(2, parts[2].size() - 4)),
                               Utils::stringViewTo<u16>(parts[1]));
            auto const & [it, ok] = infos.try_emplace(tmpl.uid(), std::move(tmpl));
            L2CPP_B_ASSERT(ok, "Skill '{}' ({}) has already been loaded", it->second.fullName(), it->second.id());
        }
        catch (l2cpp::Exception const & e)
        {
            SPDLOG_ERROR("Failed to load skill from '{}:{}':\n{}", path.string(), i, l2cpp::formatExceptionStack(e));
        }
    }

    void loadSkillGroups(std::unordered_map<SkillUid, SkillTemplate> & infos, std::filesystem::path const & path)
    {
        constexpr auto partsCount = 17;

        std::ifstream file(path);
        L2CPP_B_ASSERT(file, "Failed to open file '{}': {}", path.string(), std::strerror(errno));

        std::string line;
        std::vector<std::string_view> parts;
        parts.reserve(partsCount); /* skill_id, skill_level, oper_type, mp_consume, cast_range, cast_style, hit_time, is_magic,
                              ani_char, desc, icon_name, extra_eff, is_ench, ench_skill_id, hp_consume, UNK_0, UNK_1 */

        for (size_t i = 1; std::getline(file, line) && !line.empty(); ++i) try
        {
            boost::algorithm::split(parts, line, boost::algorithm::is_any_of("\t"));
            L2CPP_B_ASSERT(parts.size() == partsCount,
                           "Found {} parts instead of {}:\nParts found: {::?}", parts.size(), partsCount, parts);

            auto const id  = Utils::stringViewTo<SkillId>(parts[0]);
            auto const lvl = Utils::stringViewTo<SkillLevel>(parts[1]);

            auto const it = infos.find({id, lvl});
            if (it == infos.end())
            {
                SPDLOG_WARN("Skill {} lv. {} has not been loaded", id, lvl);
                continue;
            }

            auto & skill = it->second;

            std::chrono::duration<double> const duration{Utils::stringViewTo<double>(parts[6])};
            skill.setCastDuration(std::chrono::floor<std::chrono::milliseconds>(duration));
            skill.setIsMagic(parts[7] == "1");

            enum class SkillOperateType { ActiveTarget, ActiveNoTarget, Passive, Toggle };
            switch (Utils::stringViewTo<SkillOperateType>(parts[2]))
            {
                case SkillOperateType::ActiveTarget:
                case SkillOperateType::ActiveNoTarget:
                    skill.setType(SkillType::Active);
                    break;

                case SkillOperateType::Passive:
                    skill.setType(SkillType::Passive);
                    break;

                case SkillOperateType::Toggle:
                    skill.setType(SkillType::Toggle);
                    skill.setTargetNature(SkillTargetNature::Self);
                    break;
            }

            using enum SkillTargetNature;

            /**/ if (id == 18) // Hate Aura
            {
                skill.setTargetType(SkillTargetType::Aura);
                skill.setTargetNature(Ennemy);
            }
            else if (id == 78) // War Cry
            {
                skill.setTargetNature(Self);
                skill.addAbnormalEffectFactory<BuffEffectFactory>(20s, StatId::PAtkMultiplier, .2);
            }
            else if (id == 129) // Poison
            {
                skill.setTargetType(SkillTargetType::Single);
                skill.setTargetNature(Ennemy);
                skill.addAbnormalEffectFactory<DamageEffectFactory>(DamageElementType::Poison, 72, 30s, 3s);
            }
            else if (id == 1016) // Resurrection
            {
                skill.setTargetType(SkillTargetType::Single);
                skill.setTargetNature(Corpse | Character);
                skill.addAbnormalEffectFactory<ResurrectionEffectFactory>();
            }
            else if (id == 1027) // Group Heal
            {
                skill.setTargetType(SkillTargetType::Aura);
                skill.setTargetNature(Self | Party | Character);
                skill.addAbnormalEffectFactory<HealEffectFactory>(66);
            }
            else if (id == 1177) // Wind Strike
            {
                skill.setTargetType(SkillTargetType::Single);
                // skill.setTargetNature(Ennemy);
                // skill.addAbnormalEffectFactory<DamageEffectFactory>(DamageElementType::Wind, 12);
                skill.setTargetNature(Self | Ennemy);
                skill.addAbnormalEffectFactory<DamageEffectFactory>(DamageElementType::Wind, 120);
            }
            else if (id == 1204) // Wind Walk
            {
                skill.setTargetType(SkillTargetType::Single);
                skill.setTargetNature(Self | Friendly | Ennemy);
                skill.addAbnormalEffectFactory<BuffEffectFactory>(20s, StatId::MoveSpeedBonus, 33);
            }
            else if (id == 1216) // Self Heal
            {
                skill.setTargetType(SkillTargetType::Self);
                skill.addAbnormalEffectFactory<HealEffectFactory>(42);
            }
            else if (id == 1217) // Greater Heal
            {
                skill.setTargetType(SkillTargetType::Single);
                skill.setTargetNature(Self | Friendly | Ennemy);
                skill.addAbnormalEffectFactory<HealEffectFactory>(371);
            }
            else if (id == 1229) // Chant of Life
            {
                skill.setTargetType(SkillTargetType::Aura);
                skill.setTargetNature(Self | Party);
                skill.addAbnormalEffectFactory<HealEffectFactory>(12, 15s, 1s);
            }
            else if (id == 1254) // Mass Resurrection
            {
                skill.setTargetType(SkillTargetType::Multiple);
                skill.setTargetNature(Corpse | Character);
                skill.addAbnormalEffectFactory<ResurrectionEffectFactory>();
            }
            else if (id == 1256) // Heart of Paagrio
            {
                skill.setTargetType(SkillTargetType::Aura);
                skill.setTargetNature(Self | Party | Clan | Alliance);
                skill.addAbnormalEffectFactory<HealEffectFactory>(91);
                skill.addAbnormalEffectFactory<HealEffectFactory>(31, 15s, 1s);
            }
            else if (id == 1295) // Aqua Splash
            {
                skill.setTargetType(SkillTargetType::Multiple);
                skill.setTargetNature(Ennemy);
                skill.addAbnormalEffectFactory<DamageEffectFactory>(DamageElementType::Water, 41);
            }
            else if (id == 7029) // Super Haste
            {
                skill.setType(SkillType::Toggle); // Enforce toggle mode for this one because it makes more sense
                skill.addAbnormalEffectFactory<ToggleBuffEffectFactory>(StatId::MoveSpeedMultiplier, 5);
                skill.addAbnormalEffectFactory<ToggleBuffEffectFactory>(StatId::PAtkSpeedMultiplier, 5);
                skill.addAbnormalEffectFactory<ToggleBuffEffectFactory>(StatId::MAtkSpeedMultiplier, 5);
            }
        }
        catch (l2cpp::Exception const & e)
        {
            SPDLOG_ERROR("Failed to load skill from '{}:{}':\n{}", path.string(), i, l2cpp::formatExceptionStack(e));
        }
        // cast_style: 0=instant/passive 1=cast_time_non_zero 2=magic_skills_only 3=physical_skills_only
        //             4=hp_drain_magic 5=bow_skill 6=seal_of_ruler 7=physical_projectile 8=double_attack
        //             9=dual_weapon(dual/fists) 10=force_discharge
    }
}

void SkillTemplateDirectory::load(std::filesystem::path const & skillNamesFile,
                                  std::filesystem::path const & skillGroupsFile)
{
    loadSkillNames(_templates, skillNamesFile);
    loadSkillGroups(_templates, skillGroupsFile);
}

auto SkillTemplateDirectory::size() -> size_t { return _templates.size(); }

auto SkillTemplateDirectory::skill(SkillUid const uid) -> OptRef<SkillTemplate>
{
    auto const it = _templates.find(uid);
    return it != _templates.end() ? OptRef(it->second) : std::nullopt;
}
