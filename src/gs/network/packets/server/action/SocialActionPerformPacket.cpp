/// @author    Chnossos
/// @date      Created on 2026-04-28

#include "SocialActionPerformPacket.hpp"

// Project includes
#include <gs/game/actor/Actor.hpp>

using Network::Packets::Server::SocialActionPerformPacket;

SocialActionPerformPacket::SocialActionPerformPacket(Actor const & performer, SocialActionId const actionId)
    : Packet(0x2d, "SocialActionPerform")
{
    *this
        << performer.id()
        << actionId
    ;
}
