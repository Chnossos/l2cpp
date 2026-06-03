/// @author    Chnossos
/// @date      Created on 2026-04-16

#include "Stats.hpp"

// Project includes
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/CharacterStatus.hpp>
#include <gs/game/effects/BuffEffect.hpp>

// C++ includes
#include <algorithm>
#include <ranges>

using enum StatId;

Stats::Stats()
{
    (*this)[BaseHpRegen] = 1;
    (*this)[BaseMpRegen] = 1;
    (*this)[BaseCpRegen] = 1;
}

void Stats::compute(Actor const & a)
{
    reset(a);

#define STAT(s)                 (*this)[s]
#define CALCULATE_CORE_STAT(s)  STAT(s) = STAT(Base##s) + STAT(Bonus##s           )
#define CALCULATE_STAT(s)       STAT(s) = STAT(Base##s) * STAT(s##Multiplier      ) + STAT(s##Bonus)
#define CALCULATE_SPEED_STAT(s) STAT(s) = STAT(Base##s) * STAT(MoveSpeedMultiplier) + STAT(MoveSpeedBonus)

    CALCULATE_CORE_STAT(Str); CALCULATE_CORE_STAT(Dex); CALCULATE_CORE_STAT(Con);
    CALCULATE_CORE_STAT(Int); CALCULATE_CORE_STAT(Wit); CALCULATE_CORE_STAT(Men);

    if (a.type() == ActorType::Character)
    {
        auto & c = static_cast<Character const &>(a);
        if (auto const status = c.component<CharacterStatus>())
        {
            auto const level = status->level();

            STAT(MaxHpBonus)      += level * STAT(HpFlatPerLevel);
            STAT(MaxMpBonus)      += level * STAT(MpFlatPerLevel);
            STAT(MaxCpBonus)      += level * STAT(CpFlatPerLevel);
            STAT(MaxHpMultiplier) += level * STAT(HpMultiplierPerLevel);
            STAT(MaxMpMultiplier) += level * STAT(MpMultiplierPerLevel);
            STAT(MaxCpMultiplier) += level * STAT(CpMultiplierPerLevel);
        }
    }

    CALCULATE_STAT(MaxHp); CALCULATE_STAT(HpRegen);
    CALCULATE_STAT(MaxMp); CALCULATE_STAT(MpRegen);
    CALCULATE_STAT(MaxCp); CALCULATE_STAT(CpRegen);

    CALCULATE_STAT(PAtk);      CALCULATE_STAT(PDef);
    CALCULATE_STAT(MAtk);      CALCULATE_STAT(MDef);
    CALCULATE_STAT(PAtkSpeed); CALCULATE_STAT(MAtkSpeed);
    CALCULATE_STAT(PAtkRange); CALCULATE_STAT(PAtkRandom);
    CALCULATE_STAT(Accuracy);  CALCULATE_STAT(Evasion);
    CALCULATE_STAT(PCritRate); CALCULATE_STAT(MCritRate);

    CALCULATE_STAT(MoveSpeed);
    CALCULATE_SPEED_STAT(RunSpeed),     CALCULATE_SPEED_STAT(WalkSpeed),
    // CALCULATE_SPEED_STAT(SwimRunSpeed), CALCULATE_SPEED_STAT(SwimWalkSpeed),
    // CALCULATE_SPEED_STAT(FlyRunSpeed),  CALCULATE_SPEED_STAT(FlyWalkSpeed),
    STAT(SwimRunSpeed) = STAT(RunSpeed);
    STAT(FlyRunSpeed)  = STAT(RunSpeed);

    CALCULATE_STAT(MaxWeight);

    CALCULATE_STAT(InventoryLimit);
    CALCULATE_STAT(WharehouseLimit);
    CALCULATE_STAT(FreightLimit);
    CALCULATE_STAT(StoreLimit);
    CALCULATE_STAT(CommonCraftLimit);
    CALCULATE_STAT(DwarvenCraftLimit);

#undef CALCULATE_CORE_STAT
#undef CALCULATE_STAT
#undef CALCULATE_SPEED_STAT

    STAT(PAtkSpeed) = std::min(STAT(PAtkSpeed), 1500.);
    STAT(MAtkSpeed) = std::min(STAT(MAtkSpeed), 1999.);

    if (a.type() != ActorType::Character || static_cast<Character const &>(a).accessLevel == 0) // GMs can speed up
        STAT(RunSpeed) = std::min(STAT(RunSpeed), 250.);
}

void Stats::regenHpFully() { (*this)[CurHp] = (*this)[MaxHp];                }
void Stats::regenMpFully() { (*this)[CurMp] = (*this)[MaxMp];                }
void Stats::regenCpFully() { (*this)[CurCp] = (*this)[MaxCp];                }
void Stats::regenFully()   { regenHpFully(); regenMpFully(); regenCpFully(); }

void Stats::reset(Actor const & actor)
{
#define RESET_STAT(stat) (*this)[stat##Multiplier] = 1.; (*this)[stat##Bonus] = 0

    RESET_STAT(MaxHp); RESET_STAT(HpRegen);
    RESET_STAT(MaxMp); RESET_STAT(MpRegen);
    RESET_STAT(MaxCp); RESET_STAT(CpRegen);

    RESET_STAT(PAtk);      RESET_STAT(PDef);
    RESET_STAT(MAtk);      RESET_STAT(MDef);
    RESET_STAT(PAtkSpeed); RESET_STAT(MAtkSpeed);
    RESET_STAT(PAtkRange); RESET_STAT(PAtkRandom);
    RESET_STAT(Accuracy);  RESET_STAT(Evasion);
    RESET_STAT(PCritRate); RESET_STAT(MCritRate);

    RESET_STAT(MoveSpeed);

    RESET_STAT(MaxWeight);

    RESET_STAT(InventoryLimit);
    RESET_STAT(WharehouseLimit);
    RESET_STAT(FreightLimit);
    RESET_STAT(StoreLimit);
    RESET_STAT(CommonCraftLimit);
    RESET_STAT(DwarvenCraftLimit);

#undef RESET_STAT

    computeSkillEffects(actor);
}

void Stats::computeSkillEffects(Actor const & actor)
{
    auto const & effects = actor.effects();

    using namespace std::ranges::views;
    auto v = effects
           | filter([] (auto const & e) { return e->type() == EffectType::Buff; })
           | transform([] (auto const & e) -> BuffEffect const & { return static_cast<BuffEffect const &>(*e); });

    for (auto const & e : v)
        (*this)[e.modifiedStat()] += e.modifiedStatValue();
}
