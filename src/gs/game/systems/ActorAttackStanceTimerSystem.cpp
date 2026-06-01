/// @author    Chnossos
/// @date      Created on 2026-03-25

#include "ActorAttackStanceTimerSystem.hpp"

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/components/AttackStanceTimer.hpp>
#include <gs/network/packets/server/combat/AttackStanceTogglePacket.hpp>
#include <gs/utils/Chrono.hpp>

namespace
{
    constexpr ClockDuration attackStanceTimeout = 5s;
}

void ActorAttackStanceTimerSystem::updateImpl(ClockDuration const elapsed, Actor & actor)
{
    if (OptRef const timer = actor.component<AttackStanceTimer>())
    {
        if (Utils::Chrono::thresholdCrossed(timer->elapsedSinceStart, elapsed, attackStanceTimeout))
        {
            if (actor.state == ActorState::CombatIdle)
                actor.state = ActorState::Idle;

            World::broadcastAround(actor, Network::Packets::Server::AttackStanceTogglePacket(false, actor), true);

            actor.delComponent<AttackStanceTimer>();
        }
        else
            timer->elapsedSinceStart += elapsed;
    }
}
