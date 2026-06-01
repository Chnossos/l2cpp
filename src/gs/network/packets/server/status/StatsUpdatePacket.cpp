/// @author    Chnossos
/// @date      Created on 2026-04-02

#include "StatsUpdatePacket.hpp"

// Project includes
#include "../../../../game/actor/Actor.hpp"

using Network::Packets::Server::StatsUpdatePacket;

StatsUpdatePacket::StatsUpdatePacket(Actor const & emitter)
    : Packet(0x0e, "StatsUpdate")
{
    *this << emitter.id();

    appendCounterAndStoreOffset(_statsCountOffset);
}

auto StatsUpdatePacket::count() const -> size_t { return counterAtOffset(_statsCountOffset); }

auto StatsUpdatePacket::addStat(Stat const id, s32 const value) -> Packet &
{
    counterAtOffset(_statsCountOffset) += 1;
    return *this << id << value;
}

auto StatsUpdatePacket::addStat(StatId const id, StatValue const value) -> Packet &
{
    Stat s;
    switch (id)
    {
#define CASE(stat) case StatId::stat: s = Stat::stat; break
        CASE(Str);
        CASE(Dex);
        CASE(Con);
        CASE(Int);
        CASE(Wit);
        CASE(Men);
        CASE(CurHp);
        CASE(MaxHp);
        CASE(CurMp);
        CASE(MaxMp);
        CASE(CurWeight);
        CASE(MaxWeight);
        CASE(PAtk);
        CASE(PDef);
        CASE(Evasion);
        CASE(Accuracy);
        CASE(PCritRate);
        CASE(MAtk);
        CASE(MAtkSpeed);
        CASE(MDef);
        CASE(CurCp);
        CASE(MaxCp);
#undef  CASE

        default:
            return *this;
    }

    return addStat(s, static_cast<s32>(value));
}
