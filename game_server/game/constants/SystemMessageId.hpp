/// @author    Chnossos
/// @date      Created on 2026-04-28

#pragma once

// Project includes
#include "../../Typedefs.hpp"

enum class SystemMessageId : u32
{
    DisconnectedFromServer   =    0,
    WelcomeToTheWorldOfL2    =   34,
    YouHitFor_1_Damage       =   35,
    _1_HitYouFor_2_Damage    =   36,
    Earned_1_Xp              =   45,
    Use_1                    =   46,
    Using_1                  =   47,
    YouAreEquippedWith_1     =   49,
    Earned_1_XpAnd_2_Sp      =   95,
    YourLevelHasIncreased    =   96,
    Earned_1_Sp              =  331,
    _1_HasBeenDisarmed       =  417,
    _1_2                     =  614,
    _1_HpHaveBeenRestored    = 1066,
    _2sHpHasBeenRestoredBy_1 = 1067,
    MagicCriticalHit         = 1280,
};
