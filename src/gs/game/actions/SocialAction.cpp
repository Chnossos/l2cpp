/// @author    Chnossos
/// @date      Created on 2026-05-22

#include "SocialAction.hpp"

// Project includes
#include "../../network/packets/server/action/SocialActionPerformPacket.hpp"
#include "../World.hpp"
#include "../actor/Actor.hpp"

#include <common/core/Exception.hpp>
#include <common/utils/Enum.hpp>

SocialAction::SocialAction(Actor & performer, SocialActionId const actionId)
    : Action{ActionType::Social, performer}
    , _actionId{actionId}
{}

void SocialAction::onStarted()
{
    using enum ActorState;
    L2CPP_B_ASSERT(Utils::Enum::isAnyOf(performer().state, Idle, CombatIdle),
                   "Cannot perform social action in this state");

    using Network::Packets::Server::SocialActionPerformPacket;
    World::broadcastAround(performer(), SocialActionPerformPacket{performer(), _actionId}, true);
}
