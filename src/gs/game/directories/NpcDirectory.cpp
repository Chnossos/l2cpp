/// @author    Chnossos
/// @date      Created on 2026-04-23

#include "NpcDirectory.hpp"

// Project includes
#include <common/core/Exception.hpp>
#include <gs/game/components/NpcAppearance.hpp>
#include <gs/utils/Conversion.hpp>

// Third-pary includes
#include <boost/algorithm/string/predicate.hpp>

// C++ includes
#include <ranges>

using Storage = std::unordered_map<size_t, NpcInfo>;

void NpcDirectory::load()
{
    Orm::loadNpcs();
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

auto NpcDirectory::find(Npc const & npc) -> NpcInfo const &
{
    return (npc.type() == ActorType::Monster ? _monsters : _npcs).at(npc.appearance().id());
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

auto NpcDirectory::maxLevel() -> u32
{
    return _maxLevel;
}

Storage NpcDirectory::_monsters;
Storage NpcDirectory::_npcs;
u32     NpcDirectory::_maxLevel;
