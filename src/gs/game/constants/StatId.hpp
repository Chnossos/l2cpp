/// @author    Chnossos
/// @date      Created on 2026-04-16

#pragma once

// Project includes
#include <common/utils/Enum.hpp>

#define CORE_STAT(name)  name, Base##name, Bonus##name
#define STAT(name)       name, Base##name, name##Multiplier, name##Bonus
#define SPEED_STAT(name) name, Base##name

enum class StatId
{
    CORE_STAT(Str), CORE_STAT(Dex), CORE_STAT(Con),
    CORE_STAT(Int), CORE_STAT(Wit), CORE_STAT(Men),

    CurHp,  STAT(MaxHp), STAT(HpRegen), HpFlatPerLevel, HpMultiplierPerLevel,
    CurMp,  STAT(MaxMp), STAT(MpRegen), MpFlatPerLevel, MpMultiplierPerLevel,
    CurCp,  STAT(MaxCp), STAT(CpRegen), CpFlatPerLevel, CpMultiplierPerLevel,

    STAT(PAtk),      STAT(PDef),
    STAT(MAtk),      STAT(MDef),
    STAT(PAtkSpeed), STAT(MAtkSpeed),
    STAT(PAtkRange), STAT(PAtkRandom),
    STAT(Accuracy),  STAT(Evasion),
    STAT(PCritRate), STAT(MCritRate),

    STAT(MoveSpeed),
    SPEED_STAT(RunSpeed),     SPEED_STAT(WalkSpeed),
    SPEED_STAT(SwimRunSpeed), SPEED_STAT(SwimWalkSpeed),
    SPEED_STAT(FlyRunSpeed),  SPEED_STAT(FlyWalkSpeed),

    CurWeight, STAT(MaxWeight),

    STAT(InventoryLimit),
    STAT(WharehouseLimit),
    STAT(FreightLimit),
    STAT(StoreLimit),
    STAT(CommonCraftLimit),
    STAT(DwarvenCraftLimit),

    Count,
};

constexpr bool isVisualSpeedStat(StatId const id)
{
    return Utils::Enum::isAnyOf(id, StatId::MoveSpeed, StatId::PAtkSpeed);
}

#undef SPEED_STAT
#undef STAT
#undef CORE_STAT
