/// @author    Chnossos
/// @date      Created on 2026-04-23

#include "NpcDirectory.hpp"

// Project includes
#include "../../utils/Conversion.hpp"

#include <common/core/Exception.hpp>

// Third-pary includes
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <spdlog/spdlog.h>

// C++ includes
#include <fstream>
#include <ranges>

using Storage = std::unordered_map<size_t, NpcInfo>;

namespace
{
    void loadNpcNames(Storage & npcs, Storage & monsters, std::filesystem::path const & path)
    {
        constexpr auto partsCount = 7;

        std::ifstream file(path);
        L2CPP_B_ASSERT(file, "Failed to open file '{}': {}", path.string(), std::strerror(errno));

        std::string line;
        std::vector<std::string_view> parts;
        parts.reserve(partsCount); // id, name, description, rgb[0], rgb[1], rgb[2], reserved1

        std::getline(file, line); // skip header line

        for (size_t i = 1; std::getline(file, line) && !line.empty(); ++i) try
        {
            boost::algorithm::split(parts, line, boost::algorithm::is_any_of("\t"));
            if (parts.size() != partsCount)
                L2CPP_THROW("Found {} parts instead of {}:\nParts found: {::?}", parts.size(), partsCount, parts);

            auto const id = Utils::stringViewTo<size_t>(parts[0]);

            OptRef<NpcInfo> info;

            auto it = npcs.find(id);
            if (it != npcs.end())
                info = it->second;
            else if (it = monsters.find(id); it != monsters.end())
                info = it->second;

            L2CPP_B_ASSERT(info, "NPC id '{}' not loaded, cannot finalize loading", id);

            auto name = parts[1];
            if (name.starts_with("a,")) name = name.substr(2);
            if (name.ends_with ("\\0")) name = name.substr(0, name.size() - 2);
            info->name = boost::trim_copy(name);

            auto title = parts[2];
            if (title.starts_with("a,")) title = title.substr(2);
            if (title.ends_with ("\\0")) title = title.substr(0, title.size() - 2);
            info->title = boost::trim_copy(title);

            u32 titleColor = 0;
            for (size_t idx = 5; idx >= 3; --idx)
                titleColor = (titleColor | Utils::stringViewTo<u8>(parts[idx], 16)) << 8;

            info->titleColor = titleColor;
        }
        catch (Core::Exception const & e)
        {
            SPDLOG_ERROR("Failed to load skill from '{}:{}':\n{}", path.string(), i, Core::formatExceptionStack(e));
        }
    }

    void loadNpcGroups(Storage & npcs, Storage & monsters, std::filesystem::path const & path)
    {
        constexpr auto partsCount = 93;

        std::ifstream file(path);
        L2CPP_B_ASSERT(file, "Failed to open file '{}': {}", path.string(), std::strerror(errno));

        std::string line;
        std::vector<std::string_view> parts;
        parts.reserve(partsCount);

        std::getline(file, line); // skip header line

        for (size_t i = 1; std::getline(file, line) && !line.empty(); ++i) try
        {
            boost::algorithm::split(parts, line, boost::algorithm::is_any_of("\t"));
            L2CPP_B_ASSERT(parts.size() == partsCount,
                           "Found {} parts instead of {}:\nParts found: {::?}", parts.size(), partsCount, parts);

            NpcInfo info {
                .id        = Utils::stringViewTo<u32>(parts[0]),
                .type      = parts[1].contains("NPC") ? ActorType::Npc : ActorType::Monster,
                .baseSpeed = 100 * Utils::stringViewTo<double>(parts[21]),
            };

            (info.type == ActorType::Npc ? npcs : monsters).try_emplace(info.id, std::move(info));
        }
        catch (Core::Exception const & e)
        {
            SPDLOG_ERROR("Failed to load NPC from '{}:{}':\n{}", path.string(), i, Core::formatExceptionStack(e));
        }
    }
}

void NpcDirectory::load(std::filesystem::path const & npcNamesFile, std::filesystem::path const & npcGroupsFile)
{
    loadNpcGroups(_npcs, _monsters, npcGroupsFile);
    loadNpcNames (_npcs, _monsters, npcNamesFile);
}

auto NpcDirectory::npcCount()     -> size_t { return _npcs.size();                    }
auto NpcDirectory::monsterCount() -> size_t { return _monsters.size();                }
auto NpcDirectory::totalCount()   -> size_t { return _monsters.size() + _npcs.size(); }

auto NpcDirectory::find(std::string_view const name) -> std::vector<Ref<NpcInfo const>>
{
    std::vector<Ref<NpcInfo const>> result;

    for (auto * storage : {&_npcs, &_monsters})
    {
        for (auto const & info : *storage | std::views::values)
        {
            if (boost::iequals(info.name, name))
                result.emplace_back(info);
        }
    }

    return result;
}

auto NpcDirectory::find(std::wstring_view const wName) -> std::vector<Ref<NpcInfo const>>
{
    return find(Utils::toString(wName));
}

auto NpcDirectory::find(size_t const id) -> OptRef<NpcInfo const>
{
    auto info = npc(id);
    return info ? info : monster(id);
}

auto NpcDirectory::npc(size_t const id) -> OptRef<NpcInfo const>
{
    auto const it = _npcs.find(id);
    return it != _npcs.end() ? OptRef(it->second) : std::nullopt;
}

auto NpcDirectory::monster(size_t const id) -> OptRef<NpcInfo const>
{
    auto const it = _monsters.find(id);
    return it != _monsters.end() ? OptRef(it->second) : std::nullopt;
}

Storage NpcDirectory::_monsters;
Storage NpcDirectory::_npcs;
