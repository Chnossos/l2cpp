/// @author    Chnossos
/// @date      Created on 2026-04-02

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/Typedefs.hpp>
#include <gs/game/constants/StatId.hpp>

class Actor;

enum class Stat
{
    Level      = 0x01,
    Xp         = 0x02,
    Str        = 0x03,
    Dex        = 0x04,
    Con        = 0x05,
    Int        = 0x06,
    Wit        = 0x07,
    Men        = 0x08,
    CurHp      = 0x09,
    MaxHp      = 0x0a,
    CurMp      = 0x0b,
    MaxMp      = 0x0c,
    Sp         = 0x0d,
    CurWeight  = 0x0e,
    MaxWeight  = 0x0f,

    PAtk       = 0x11,
    PAtkSpeed  = 0x12,
    PDef       = 0x13,
    Evasion    = 0x14,
    Accuracy   = 0x15,
    PCritRate  = 0x16,
    MAtk       = 0x17,
    MAtkSpeed  = 0x18,
    MDef       = 0x19,
    PvpFlag    = 0x1a,
    Karma      = 0x1b,

    CurCp      = 0x21,
    MaxCp      = 0x22,
};

namespace Network::Packets::Server { class StatsUpdatePacket; }

class Network::Packets::Server::StatsUpdatePacket final : public Network::Packet
{
public:
    explicit StatsUpdatePacket(Actor const & emitter);

public:
    auto count() const -> size_t;
    bool empty() const { return count() == 0; }

public:
    auto addStat(Stat id, s32 value) -> Packet &;
    auto addStat(StatId id, StatValue value) -> Packet &;

    template<typename T> requires std::integral<T> || std::floating_point<T>
    auto addStat(Stat const id, T const value) -> Packet & { return addStat(id, static_cast<s32>(value)); }

private:
    u32 _statsCountOffset;
};
