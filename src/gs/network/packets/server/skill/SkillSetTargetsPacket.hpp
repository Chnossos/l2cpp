/// @author    Chnossos
/// @date      Created on 2026-04-08

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Actor;
class SkillTemplate;

namespace Network::Packets::Server { struct SkillSetTargetsPacket; }

struct Network::Packets::Server::SkillSetTargetsPacket final : public Network::Packet
{
    SkillSetTargetsPacket(Actor const & caster, SkillTemplate const & skill, std::span<Ref<Actor const> const> targets);
};
