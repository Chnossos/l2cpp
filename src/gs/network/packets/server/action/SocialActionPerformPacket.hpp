/// @author    Chnossos
/// @date      Created on 2026-04-28

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/game/constants/SocialActionId.hpp>

class Actor;

namespace Network::Packets::Server { struct SocialActionPerformPacket; }

struct Network::Packets::Server::SocialActionPerformPacket final : public Packet
{
    SocialActionPerformPacket(Actor const & performer, SocialActionId actionId);
};
