/// @author    Chnossos
/// @date      Created on 2026-03-31

#include "SkillUid.hpp"

// Project includes
#include "../../network/packets/server/status/EffectListPacket.hpp"

DEFINE_PACKET_SERIALIZATION_OPERATOR(SkillUid, uid)
{
    return p << static_cast<u32>(uid.id()) << static_cast<u32>(uid.level());
}

DEFINE_CUSTOM_PACKET_SERIALIZATION_OPERATOR(Network::Packet::Server::EffectListPacket, SkillUid, uid)
{
    return p << static_cast<u32>(uid.id()) << uid.level();
}
