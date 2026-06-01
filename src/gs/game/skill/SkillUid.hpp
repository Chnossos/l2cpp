/// @author    Chnossos
/// @date      Created on 2026-03-11

#pragma once

// Project includes
#include "../../Typedefs.hpp"

#include <common/network/Serialization.hpp>

namespace Network::Packets::Server { struct EffectListPacket; }

class SkillUid
{
    DECLARE_PACKET_SERIALIZATION_OPERATOR(SkillUid);
    DECLARE_CUSTOM_PACKET_SERIALIZATION_OPERATOR(Network::Packets::Server::EffectListPacket, SkillUid);

public:
    SkillUid(SkillId const id, SkillLevel const level) noexcept: _uid(id | level << 24) {}

public:
    auto id()    const -> SkillId    { return static_cast<SkillId>   (_uid & 0x00FFFFFF); }
    auto level() const -> SkillLevel { return static_cast<SkillLevel>(_uid >> 24 & 0xFF); }

public:
    // ReSharper disable once CppNonExplicitConversionOperator
    operator u32() const { return _uid; }

private:
    u32 _uid;
};

// ReSharper disable once CppRedundantNamespaceDefinition
namespace std
{
    template<>
    struct hash<SkillUid>
    {
        size_t operator()(SkillUid const uid) const noexcept
        {
            return std::hash<u32>{}(uid);
        }
    };
}
