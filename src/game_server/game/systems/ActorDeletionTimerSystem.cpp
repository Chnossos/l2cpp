/// @author    Chnossos
/// @date      Created on 2026-04-05

#include "ActorDeletionTimerSystem.hpp"

// Project includes
#include "../World.hpp"
#include "../components/DeletionTimer.hpp"

void ActorDeletionTimerSystem::updateImpl(ClockDuration const elapsed, Actor & actor)
{
    if (auto const timer = actor.component<DeletionTimer>())
    {
        if ((timer->elapsedSinceDeath += elapsed) >= timer->timeBeforeDeletion)
            World::scheduleForDeletion(actor);
    }
}
