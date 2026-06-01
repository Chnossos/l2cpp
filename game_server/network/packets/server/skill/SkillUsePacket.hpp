/// @author    Chnossos
/// @date      Created on 2026-04-08

#pragma once

// Project includes
#include "../../../../game/skill/SkillUid.hpp"

#include <l2cpp/network/Packet.hpp>

class Actor;
class Skill;

namespace Network::Packets::Server { struct SkillUsePacket; }

struct Network::Packets::Server::SkillUsePacket final : public Network::Packet
{
    SkillUsePacket(Actor const & caster, Actor const & target, SkillUid skillUid,
                   ClockDuration castDuration, ClockDuration cooldown, bool isCritical);
};
