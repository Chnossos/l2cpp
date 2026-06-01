/// @author    Chnossos
/// @date      Created on 2026-04-08

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct SkillCancelPacket; }

struct Network::Packets::Server::SkillCancelPacket final : public Network::Packet
{
    explicit SkillCancelPacket(Actor const & caster);
};
