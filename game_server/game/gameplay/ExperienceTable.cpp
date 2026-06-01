/// @author    Chnossos
/// @date      Created on 2026-04-28

#include "ExperienceTable.hpp"

// Project includes
#include <l2cpp/core/Exception.hpp>

// C++ includes
#include <array>

inline constexpr std::array gExperienceTable{
                0u, // level 0 (unreachable)
                0u, // level 1
               68u,
              363u,
            1'168u,
            2'884u,
            6'038u,
           11'287u,
           19'423u,
           31'378u,
           48'229u, // level 10
           71'201u,
          101'676u,
          141'192u,
          191'452u,
          254'327u,
          331'864u,
          426'284u,
          539'995u,
          675'590u,
          835'854u,  // level 20
        1'023'775u,
        1'242'536u,
        1'495'531u,
        1'786'365u,
        2'118'860u,
        2'497'059u,
        2'925'229u,
        3'407'873u,
        3'949'727u,
        4'555'766u, // level 30
        5'231'213u,
        5'981'539u,
        6'812'472u,
        7'729'999u,
        8'740'372u,
        9'850'111u,
       11'066'012u,
       12'395'149u,
       13'844'879u,
       15'422'851u, // level 40
       17'137'002u,
       18'995'573u,
       21'007'103u,
       23'180'442u,
       25'524'751u,
       28'049'509u,
       30'764'519u,
       33'679'907u,
       36'806'133u,
       40'153'995u, // level 50
       45'524'865u,
       51'262'204u,
       57'383'682u,
       63'907'585u,
       70'852'742u,
       80'700'339u,
       91'162'131u,
      102'265'326u,
      114'038'008u,
      126'509'030u, // level 60
      146'307'211u,
      167'243'291u,
      189'363'788u,
      212'716'741u,
      237'351'413u,
      271'973'532u,
      308'441'375u,
      346'825'235u,
      387'197'529u,
      429'632'402u, // level 70
      474'205'751u,
      532'692'055u,
      606'319'094u,
      696'376'867u,
      804'219'972u,
      931'275'828u,
    1'151'275'834u,
    1'511'275'834u, // level 78
    // Unreachable with normal means; listed for completeness
    2'099'275'834u, // level 79 (decrement by 1 to get to lv. 78 100%)
    2'099'325'832u, // level 80
};

auto ExperienceTable::level(u32 const experience) -> u32
{
    u32 level = 2;

    while (level < gExperienceTable.size() && experience >= gExperienceTable[level])
        ++level;

    return level - 1;
}

auto ExperienceTable::floor(u32 const level) -> u32
{
    L2CPP_B_ASSERT(minLevel() <= level && level <= maxLevel(), "Invalid level '{}'", level);
    return gExperienceTable[level];
}

auto ExperienceTable::ceil(u32 const level) -> u32
{
    L2CPP_B_ASSERT(minLevel() <= level && level <= maxLevel(), "Invalid level '{}'", level);
    return gExperienceTable[level + 1] - 1;
}

auto ExperienceTable::minLevel() -> u32 { return 1;                                             }
auto ExperienceTable::maxLevel() -> u32 { return static_cast<u32>(gExperienceTable.size()) - 3; }
