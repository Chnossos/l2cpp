/// @author    Chnossos
/// @date      Created on 2026-03-25

#include "ActorAttackStanceTimerSystem.hpp"

// Project includes
#include "../World.hpp"
#include "../../network/packets/server/combat/AttackStanceTogglePacket.hpp"
#include "../../utils/Chrono.hpp"
#include "../components/AttackStanceTimer.hpp"

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

            World::broadcastAround(actor, Network::Packet::Server::AttackStanceTogglePacket(false, actor), true);

            actor.delComponent<AttackStanceTimer>();
        }
        else
            timer->elapsedSinceStart += elapsed;
    }
}
