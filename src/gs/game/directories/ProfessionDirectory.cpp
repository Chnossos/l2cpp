/// @author    Chnossos
/// @date      Created on 2026-06-03

#include "ProfessionDirectory.hpp"

// Project includes
#include <gs/game/actor/Actor.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/game/constants/StatId.hpp>

void ProfessionDirectory::ProfessionInfo::applyBaseStats(Actor & actor) const
{
    using enum StatId;
    auto & stats = actor.stats();
    stats[BaseMaxHp           ] = maxHp;
    stats[BaseMaxMp           ] = maxMp;
    stats[BaseMaxCp           ] = maxCp;
    stats[HpFlatPerLevel      ] = hpFlatPerLevel;
    stats[MpFlatPerLevel      ] = mpFlatPerLevel;
    stats[CpFlatPerLevel      ] = cpFlatPerLevel;
    stats[HpMultiplierPerLevel] = hpMultiplierPerLevel;
    stats[MpMultiplierPerLevel] = mpMultiplierPerLevel;
    stats[CpMultiplierPerLevel] = cpMultiplierPerLevel;
    stats.compute(actor);
}

auto ProfessionDirectory::count() -> size_t
{
    return _professions.size();
}

auto ProfessionDirectory::find(Profession const profession) -> OptRef<ProfessionInfo const>
{
    auto const it = _professions.find(profession);
    return it != _professions.end() ? OptRef<ProfessionInfo const>{it->second} : std::nullopt;
}

auto ProfessionDirectory::parent(Profession const profession) -> OptRef<ProfessionInfo const>
{
    auto const info = find(profession);
    return info && info->parentProfession ? find(*info->parentProfession) : std::nullopt;
}

auto ProfessionDirectory::startingProfession(Profession const profession) -> OptRef<ProfessionInfo const>
{
    auto info = find(profession);

    while (info && info->parentProfession)
        info = find(*info->parentProfession);

    return info;
}

auto ProfessionDirectory::rank(Profession const profession) -> u8
{
    u8 rank = 0;

    for (auto info = find(profession); info && info->parentProfession; info = find(*info->parentProfession))
        ++rank;

    return rank;
}

void ProfessionDirectory::load()
{
    Orm::loadProfessions();
}

std::unordered_map<Profession, ProfessionDirectory::ProfessionInfo> ProfessionDirectory::_professions;
