/// @author    Chnossos
/// @date      Created on 2026-04-08

#include "SkillAction.hpp"

// Project includes
#include "../../network/packets/server/action/ActionFailedPacket.hpp"
#include "../../network/packets/server/skill/SkillCancelPacket.hpp"
#include "../../network/packets/server/skill/SkillSetTargetsPacket.hpp"
#include "../../network/packets/server/skill/SkillUsePacket.hpp"
#include "../../network/packets/server/ui/UiGaugePacket.hpp"
#include "../../utils/Chrono.hpp"
#include "../../utils/Target.hpp"
#include "../World.hpp"
#include "../actor/Actor.hpp"
#include "../components/Stats.hpp"
#include "../skill/Skill.hpp"

#include <l2cpp/Exception.hpp>
// ReSharper disable once CppUnusedIncludeDirective
#include <l2cpp/details/Pimpl.hpp>
#include <l2cpp/utils/Enum.hpp>

namespace SC = Network::Packet::Server;

struct SkillAction::SkillActionImpl
{
    SkillTemplate const &       skill;
    bool                        forceAttack;
    ClockDuration               castingElapsed       = ClockDuration::zero();
    ClockDuration               castingDuration      = ClockDuration::zero();
    ClockDuration               notifyTargetsTrigger = ClockDuration::zero();
    std::optional<GameObjectId> targetId;
    std::vector<Ref<Actor>>     targets;
    ActorState                  previousState;
};

template class Pimpl<SkillAction::SkillActionImpl>;

// ---------------------------------------------------------------------------------------------------------------------

SkillAction::SkillAction(Actor & performer, SkillTemplate const & skill, bool const forceAttack)
    : Action(ActionType::Skill, performer)
    , _impl(skill, forceAttack)
{
    L2CPP_B_ASSERT(l2cpp::Utils::Enum::isAnyOf(_impl->skill.type(), SkillType::Active, SkillType::Toggle),
                   "Unsupported skill type '{}'", std::to_underlying(_impl->skill.type()));

    auto const target = performer.target();
    L2CPP_B_ASSERT(!skill.needsTarget() || target, "Skill '{}' needs a target", skill.id());
    if (skill.needsTarget())
        _impl->targetId = target->id();
}

SkillAction::SkillAction(SkillAction &&) noexcept = default;
SkillAction & SkillAction::operator=(SkillAction &&) noexcept = default;
SkillAction::~SkillAction() = default;

bool SkillAction::canBeInterruptedByAnotherAction() const { return false; }

void SkillAction::onStarted()
{
    // We check everything again here because the action could've been queued, and by the time it gets executed,
    // many things may have changed.

    if (!performer().isAlive())
        return setFinished(true);

    if (_impl->skill.type() == SkillType::Toggle)
    {
        _impl->targets.emplace_back(performer());
        setFinished(true);
        return;
    }

    OptRef<Actor> target;
    if (_impl->skill.targetType() == SkillTargetType::Self)
    {
        target = performer();
    }
    else if (_impl->skill.needsTarget())
    {
        target = World::actor(*_impl->targetId);
        if (!target || !Utils::Target::isValidTarget(performer(), _impl->skill, target, _impl->forceAttack))
            return setFinished(true); // Target doesn't exist or isn't valid anymore, do nothing
    }

    if (target)
        _impl->targets.emplace_back(target);

    if (_impl->skill.castDuration() != ClockDuration::zero())
    {
        if (_impl->skill.isMagic())
        {
            auto const mAtkSpeedRatio = performer().stats()[StatId::MAtkSpeed] / 333;
            _impl->castingDuration    = Utils::Chrono::Clock::toDuration(_impl->skill.castDuration() / mAtkSpeedRatio);

            if (_impl->skill.castDuration() > 550ms && _impl->castingDuration <= 550ms) // Avoid broken animations
                _impl->castingDuration  = 550ms;
        }
        else
        {
            auto const pAtkSpeedRatio = performer().stats()[StatId::PAtkSpeed] / 300;
            _impl->castingDuration    = Utils::Chrono::Clock::toDuration(_impl->skill.castDuration() / pAtkSpeedRatio);

            if (_impl->skill.castDuration() > 500ms && _impl->castingDuration < 500ms) // Avoid broken animations
                _impl->castingDuration = 500ms;
        }

        // Skill targets must be sent 350ms (including latency) before the end of the animation to display correctly
        _impl->notifyTargetsTrigger = _impl->castingDuration - 350ms;
    }

    _impl->previousState = performer().state;
    performer().state = ActorState::Casting;

    SC::SkillUsePacket p{performer(), target ? *target : performer(), _impl->skill.uid(),
                         _impl->castingDuration, _impl->skill.cooldownDuration(), false};
    World::broadcastAround(performer(), std::move(p), true);
    World::send(performer(), SC::UiGaugePacket{GaugeColor::Blue, _impl->castingDuration});
}

void SkillAction::updateImpl(ClockDuration const elapsed)
{
    // TODO: ensure target is still valid

    using enum SkillTargetType;
    if (l2cpp::Utils::Enum::isAnyOf(_impl->skill.targetType(), AoE, Aura) &&
        Utils::Chrono::thresholdCrossed(_impl->castingElapsed, elapsed, _impl->notifyTargetsTrigger))
    {
        selectTargets();

        std::vector<Ref<Actor const>> targets;
        targets.assign_range(_impl->targets);

        World::broadcastAround(performer(),
                               SC::SkillSetTargetsPacket{performer(), _impl->skill, targets}, true);
    }

    setFinished((_impl->castingElapsed += elapsed) >= _impl->castingDuration);
}

void SkillAction::onFinished()
{
    for (auto & target : _impl->targets)
        _impl->skill.applyEffects(performer(), target);

    performer().state = _impl->previousState;
}

void SkillAction::onCanceled()
{
    World::broadcastAround(performer(), SC::SkillCancelPacket{performer()}, true);

    performer().state = _impl->previousState;
}

void SkillAction::selectTargets()
{
    auto const targetType = _impl->skill.targetType();

    using enum SkillTargetType;
    if (!l2cpp::Utils::Enum::isAnyOf(targetType, AoE, Aura))
        return;

    if (targetType == Aura && Utils::Target::isValidTarget(performer(), _impl->skill, performer(), _impl->forceAttack))
        _impl->targets.emplace_back(performer());

    auto const & target = _impl->targets.empty() ? performer() : _impl->targets.at(0).get();
    World::forEachActorAround(target, [&] (auto & a)
    {
        if (Utils::Target::isValidTarget(performer(), _impl->skill, a, _impl->forceAttack))
            _impl->targets.emplace_back(a);
    });
}
