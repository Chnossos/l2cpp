/// @author    Chnossos
/// @date      Created on 2026-03-17

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "ActorStatus.hpp"

class CharacterStatus final : public ActorStatus
{
public:
    auto level()    const -> u32 override;
    auto xp()       const -> u32;
    auto sp()       const -> u32;
    auto pvpCount() const -> u32;
    auto pkCount()  const -> u32;
    auto karma()    const -> u32;

    bool isNoblesse() const;
    bool isHero()  const;

public:
    /// @param level   Level to set this character to, between min and max of course.
    /// @param percent Between @c 0.0 and @c 100.0
    void setLevel   (u32 level, double percent = 0) override;
    void setXp      (u32 xp);
    void setSp      (u32 sp);
    void setPvpCount(u32 pvpCount);
    void setPkCount (u32 pkCount);
    void setKarma   (u32 karma);
    void addXp      (u32 xp);
    void addSp      (u32 sp);

    void setNoblesse(bool);
    void setHero(bool);

private:
    u32 _xp = 0, _sp = 0;
    u32 _pvpCount = 0, _pkCount = 0, _karma = 0;

    bool _isNoblesse = false;
    bool _isHero     = false;
};
