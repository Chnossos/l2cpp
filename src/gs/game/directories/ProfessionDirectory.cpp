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
    auto const it = _professions.find(std::to_underlying(profession));
    return it != _professions.end() ? OptRef<ProfessionInfo const>{it->second} : std::nullopt;
}

void ProfessionDirectory::load()
{
    Orm::loadProfessions();
}

std::unordered_map<u32, ProfessionDirectory::ProfessionInfo> ProfessionDirectory::_professions;
