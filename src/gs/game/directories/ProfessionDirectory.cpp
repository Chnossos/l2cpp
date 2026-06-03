/// @author    Chnossos
/// @date      Created on 2026-06-03

#include "ProfessionDirectory.hpp"

// Project includes
#include <gs/game/actor/Actor.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/game/constants/StatId.hpp>

void ProfessionDirectory::ProfessionInfo::applyStats(Actor & actor) const
{
    using enum StatId;
    auto & stats = actor.stats();
    stats[BaseStr             ] = STR;
    stats[BaseDex             ] = DEX;
    stats[BaseCon             ] = CON;
    stats[BaseInt             ] = INT;
    stats[BaseWit             ] = WIT;
    stats[BaseMen             ] = MEN;
    stats[BasePAtk            ] = pAtk;
    stats[BaseMAtk            ] = mAtk;
    stats[BasePDef            ] = pDef;
    stats[BaseMDef            ] = mDef;
    stats[BasePAtkSpeed       ] = pAtkSpeed;
    stats[BaseMAtkSpeed       ] = mAtkSpeed;
    stats[BaseRunSpeed        ] = runSpeed;
    stats[BaseWalkSpeed       ] = walkSpeed;
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
