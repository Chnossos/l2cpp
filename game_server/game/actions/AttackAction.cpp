/// @author    Chnossos
/// @date      Created on 2026-03-24

#include "AttackAction.hpp"

// Project includes
#include "../../network/packets/server/combat/AttackPacket.hpp"
#include "../../network/packets/server/combat/AttackStanceTogglePacket.hpp"
#include "../../utils/Chrono.hpp"
#include "../World.hpp"
#include "../actor/Character.hpp"
#include "../components/AttackStanceTimer.hpp"
#include "../components/Gear.hpp"
#include "../components/Stats.hpp"

#include <l2cpp/network/Packet.hpp>

namespace SM = Network::Packet::Server;

AttackAction::AttackAction(Actor & performer, Actor & target, StatValue const pAtkSpeed) noexcept
    : Action(ActionType::Attack, performer)
    , _target(target)
    , _hitDuration(Utils::Chrono::Clock::toDuration(1s / (pAtkSpeed / 500)))
{}

bool AttackAction::canBeInterruptedByAnotherAction() const { return false; }

void AttackAction::onStarted()
{
    performer().state = ActorState::Attacking;

    u8 hitCount = 1;

    std::optional<ItemGrade> soulShotGrade;
    if (performer().type() == ActorType::Character)
    {
        auto & c = static_cast<Character &>(performer());

        if (auto const weapon = c.gear().weapon())
        {
            // Use soulshots if a weapon is equipped
            soulShotGrade = weapon->tmplate.grade;

            // Dual weapon means same item in both hands
            if (auto const id = c.gear().itemId(GearSlot::LeftHand); id && weapon->id() == id)
                ++hitCount;
        }
        else if (!c.gear().item(GearSlot::LeftHand))
            ++hitCount; // bare fists have two hits when not carrying a shield
    }

    SM::AttackPacket p(performer(), _target);
    for (decltype(hitCount) i = 0; i < hitCount; ++i) // split dual hits damage
        p.addHit({_target, 250u / hitCount, false, soulShotGrade});

    World::broadcastAround(performer(), std::move(p), true);
}

void AttackAction::updateImpl(ClockDuration const)
{
    setFinished(lastUpdateTime() >= startTime() + _hitDuration);
}

void AttackAction::onFinished()
{
    performer().state = ActorState::CombatIdle;

    auto & actor = performer();
    for (auto * const a : {&actor, &_target})
    {
        if (auto const timer = a->component<AttackStanceTimer>())
            timer->restart();
        else
        {
            a->addComponent<AttackStanceTimer>().restart();
            World::broadcastAround(*a, SM::AttackStanceTogglePacket(true, *a), true);
        }
    }

    _target.takeDamage(actor, 250);

    // Physical attacking never stops unless another action is requested (e.g. actor moves) or target died
    if (!actor.nextAction() && _target.isAlive())
        actor.doNext<AttackAction>(_target, actor.stats()[StatId::PAtkSpeed]);
}
