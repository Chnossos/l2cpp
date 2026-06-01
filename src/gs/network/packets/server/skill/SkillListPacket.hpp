/// @author    Chnossos
/// @date      Created on 2026-03-11

#pragma once

// Project includes
#include "../../../../game/components/SkillDirectory.hpp"

#include <common/network/Packet.hpp>

namespace Network::Packets::Server { struct SkillListPacket; }

struct Network::Packets::Server::SkillListPacket final : public Network::Packet
{
    explicit SkillListPacket(SkillDirectory const &);
};
