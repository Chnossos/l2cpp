/// @author    Chnossos
/// @date      Created on 2026-05-22

#include "SocialAction.hpp"

// Project includes
#include <common/core/Exception.hpp>
#include <common/utils/Enum.hpp>
#include <gs/game/World.hpp>
#include <gs/game/actor/Actor.hpp>
#include <gs/network/packets/server/action/SocialActionPerformPacket.hpp>

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
