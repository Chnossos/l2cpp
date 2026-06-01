/// @author    Chnossos
/// @date      Created on 2026-04-28

#pragma once

// Project includes
#include "../../../../game/constants/SocialActionId.hpp"

#include <l2cpp/network/Packet.hpp>

class Actor;

namespace Network::Packets::Server { struct SocialActionPerformPacket; }

struct Network::Packets::Server::SocialActionPerformPacket final : public Network::Packet
{
    SocialActionPerformPacket(Actor const & performer, SocialActionId actionId);
};
