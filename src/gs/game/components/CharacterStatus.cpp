/// @author    Chnossos
/// @date      Created on 2026-05-10

#include "CharacterStatus.hpp"

// Project includes
#include <common/core/Exception.hpp>
#include <gs/game/gameplay/ExperienceTable.hpp>

auto CharacterStatus::level()      const -> u32  { return ExperienceTable::level(_xp); }
auto CharacterStatus::xp()         const -> u32  { return _xp;                         }
auto CharacterStatus::sp()         const -> u32  { return _sp;                         }
auto CharacterStatus::pvpCount()   const -> u32  { return _pvpCount;                   }
auto CharacterStatus::pkCount()    const -> u32  { return _pkCount;                    }
auto CharacterStatus::karma()      const -> u32  { return _karma;                      }
auto CharacterStatus::isNoblesse() const -> bool { return _isNoblesse;                 }
auto CharacterStatus::isHero()     const -> bool { return _isHero;                     }

void CharacterStatus::setLevel(u32 const level, double percent)
{
    L2CPP_B_ASSERT(ExperienceTable::minLevel() <= level && level <= ExperienceTable::maxLevel(),
                   "Invalid level '{}'", level);

    /**/ if (percent < 0)   percent = 0;
    else if (percent > 100) percent = 100;

    /**/ if (percent == 0)   _xp = ExperienceTable::floor(level);
    else if (percent == 100) _xp = ExperienceTable::ceil (level) + (level < ExperienceTable::maxLevel());
    else
    {
        _xp  = ExperienceTable::floor(level);
        _xp += static_cast<u32>((ExperienceTable::ceil(level) - _xp) * percent / 100);
    }
}

void CharacterStatus::setXp      (u32 const xp)       { _xp = std::min<u32>(std::numeric_limits<s32>::max(), xp); }
void CharacterStatus::setSp      (u32 const sp)       { _sp = std::min<u32>(std::numeric_limits<s32>::max(), sp); }
void CharacterStatus::setPvpCount(u32 const pvpCount) { _pvpCount = pvpCount; }
void CharacterStatus::setPkCount (u32 const pkCount)  { _pkCount  = pkCount;  }
void CharacterStatus::setKarma   (u32 const karma)    { _karma    = karma;    }

void CharacterStatus::addXp(u32 const xp)
{
    _xp += std::min(ExperienceTable::ceil(ExperienceTable::maxLevel()) - _xp, xp);
}

void CharacterStatus::addSp(u32 const sp)
{
    _sp += std::min(std::numeric_limits<s32>::max() - _sp, sp);
}

void CharacterStatus::setNoblesse(bool const b) { _isNoblesse = b; }
void CharacterStatus::setHero    (bool const b) { _isHero     = b; }
