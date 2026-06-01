/// @author    Chnossos
/// @date      Created on 2026-04-16

#include "Stats.hpp"

// Project includes
#include "../actor/Character.hpp"

Stats::Stats()
{
    using enum StatId;
#define INIT_MULTIPLIER(stat) _stats[std::to_underlying(stat##Multiplier)] = 1.

    INIT_MULTIPLIER(MaxHp); INIT_MULTIPLIER(HpRegen);
    INIT_MULTIPLIER(MaxMp); INIT_MULTIPLIER(MpRegen);
    INIT_MULTIPLIER(MaxCp); INIT_MULTIPLIER(CpRegen);

    INIT_MULTIPLIER(PAtk);      INIT_MULTIPLIER(PDef);
    INIT_MULTIPLIER(MAtk);      INIT_MULTIPLIER(MDef);
    INIT_MULTIPLIER(PAtkSpeed); INIT_MULTIPLIER(MAtkSpeed);
    INIT_MULTIPLIER(PAtkRange); INIT_MULTIPLIER(PAtkRandom);
    INIT_MULTIPLIER(Accuracy);  INIT_MULTIPLIER(Evasion);
    INIT_MULTIPLIER(PCritRate); INIT_MULTIPLIER(MCritRate);

    INIT_MULTIPLIER(MoveSpeed);

    INIT_MULTIPLIER(MaxWeight);

    INIT_MULTIPLIER(InventoryLimit);
    INIT_MULTIPLIER(WharehouseLimit);
    INIT_MULTIPLIER(FreightLimit);
    INIT_MULTIPLIER(StoreLimit);
    INIT_MULTIPLIER(CommonCraftLimit);
    INIT_MULTIPLIER(DwarvenCraftLimit);

#undef INIT_MULTIPLIER
}

void Stats::compute(Actor const & a)
{
    using enum StatId;

#define STAT(s)                 (*this)[s]
#define CALCULATE_CORE_STAT(s)  STAT(s) = STAT(Base##s) + STAT(Bonus##s           )
#define CALCULATE_STAT(s)       STAT(s) = STAT(Base##s) * STAT(s##Multiplier      ) + STAT(s##Bonus)
#define CALCULATE_SPEED_STAT(s) STAT(s) = STAT(Base##s) * STAT(MoveSpeedMultiplier) + STAT(MoveSpeedBonus)

    CALCULATE_CORE_STAT(Str); CALCULATE_CORE_STAT(Dex); CALCULATE_CORE_STAT(Con);
    CALCULATE_CORE_STAT(Int); CALCULATE_CORE_STAT(Wit); CALCULATE_CORE_STAT(Men);

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
    CALCULATE_SPEED_STAT(SwimRunSpeed), CALCULATE_SPEED_STAT(SwimWalkSpeed),
    CALCULATE_SPEED_STAT(FlyRunSpeed),  CALCULATE_SPEED_STAT(FlyWalkSpeed),

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
