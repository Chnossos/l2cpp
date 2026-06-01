/// @author    Chnossos
/// @date      Created on 2026-03-11

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/game/components/SkillDirectory.hpp>

namespace Network::Packets::Server { struct SkillListPacket; }

struct Network::Packets::Server::SkillListPacket final : public Network::Packet
{
    explicit SkillListPacket(SkillDirectory const &);
};
