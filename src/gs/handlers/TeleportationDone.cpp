/// @author    Chnossos
/// @date      Created on 2026-06-11

#pragma once

// Project includes
#include <gs/game/actions/TeleportAction.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/handlers/_Common.hpp>

DEFINE_PACKET_HANDLER(TeleportationDone)
{
    auto const c = player.currentCharacter();
    L2CPP_B_ASSERT(c, "No current player, cannot appear");

    auto const action = c->currentAction();
    L2CPP_B_ASSERT(action && action->type() == ActionType::Teleport, "This player is not teleporting");

    c->setPosition(static_cast<TeleportAction &>(*action).destination());
}
