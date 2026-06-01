/// @author    Chnossos
/// @date      Created on 2026-04-08

#include "SkillAction.hpp"

// Project includes
#include <common/core/Exception.hpp>
#include <common/details/Pimpl.hpp>
#include <common/utils/Enum.hpp>
#include <gs/game/World.hpp>
#include <gs/game/actor/Actor.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/game/skill/Skill.hpp>
#include <gs/network/packets/server/chat/ChatSystemSayPacket.hpp>
#include <gs/network/packets/server/skill/SkillCancelPacket.hpp>
#include <gs/network/packets/server/skill/SkillSetTargetsPacket.hpp>
#include <gs/network/packets/server/skill/SkillUsePacket.hpp>
#include <gs/network/packets/server/ui/UiGaugePacket.hpp>
#include <gs/utils/Chrono.hpp>
#include <gs/utils/Target.hpp>

// C++ includes
#include <algorithm>
#include <ranges>

namespace SC = Network::Packets::Server;
using enum EffectTargetType;
using enum SkillOperatingType;
using Utils::Enum::isAnyOf;
using Utils::Target::isValidTarget;

struct SkillAction::Impl
{
    SkillTemplate const &       skill;
    bool                        forceAttack, castingStarted = false;
    ClockDuration               castingElapsed       = ClockDuration::zero();
    ClockDuration               castingDuration      = ClockDuration::zero();
    ClockDuration               notifyTargetsTrigger = ClockDuration::zero();
    std::optional<GameObjectId> targetId;
    ActorState                  previousState;

    std::unordered_map<EffectTargetType, std::vector<Ref<Actor>>> targets;
};

template class Pimpl<SkillAction::Impl>;

// ---------------------------------------------------------------------------------------------------------------------

SkillAction::SkillAction(Actor & performer, SkillTemplate const & skill, bool const forceAttack)
    : Action(ActionType::Skill, performer)
    , _impl(skill, forceAttack)
{
    L2CPP_B_ASSERT(isAnyOf(_impl->skill.operatingType(), Active, Toggle),
                   "Unsupported skill oeprating type '{}'", std::to_underlying(_impl->skill.operatingType()));

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
    // We check many conditions again here because the action could've been queued, and by the time it gets executed,
    // the whole situation could've changed.

    if (!performer().isAlive())
        return setFinished(true);

    if (_impl->skill.operatingType() == Toggle)
    {
        _impl->targets[_impl->skill.targetType()].emplace_back(performer());

        // Only send message on toggled-on, not toggled-off
        auto const matchToggle = [this] (auto const & e) { return e->skillUid() == _impl->skill.uid(); };
        if (std::ranges::none_of(performer().effects(), matchToggle))
            sendUseSkillSystemMessage();

        // Toggle skills don't have a cast animation, we can skip to applying the effects
        _impl->castingStarted = true;
        return setFinished(true);
    }

    OptRef<Actor> target;
    if (_impl->skill.needsTarget())
    {
        if (_impl->targetId == performer().id())
            target = performer();
        else
            target = World::actor(*_impl->targetId);

        if (!target || !isValidTarget(performer(), _impl->skill.targetNature(), target, _impl->forceAttack))
            return setFinished(true); // Target doesn't exist or isn't valid anymore, do nothing
    }

    if (_impl->skill.castDuration() != ClockDuration::zero())
        adjustCastingDuration();

    _impl->previousState = performer().state;
    performer().state = ActorState::Casting;

    sendUseSkillSystemMessage();
    World::send(performer(), SC::UiGaugePacket{GaugeColor::Blue, _impl->castingDuration});

    bool const isCritical = false;
    if (isCritical)
        World::send(performer(), SC::ChatSystemSayPacket{SystemMessageId::MagicCriticalHit});

    SC::SkillUsePacket p{performer(), target ? *target : performer(), _impl->skill.uid(),
                         _impl->castingDuration, _impl->skill.cooldownDuration(), isCritical};
    World::broadcastAround(performer(), std::move(p), true);
    _impl->castingStarted = true;
}

void SkillAction::updateImpl(ClockDuration const elapsed)
{
    if (Utils::Chrono::thresholdCrossed(_impl->castingElapsed, elapsed, _impl->notifyTargetsTrigger))
    {
        for (auto const & effect : _impl->skill.effects())
            selectTargets(effect->targetType(), effect->targetNature());

        // Do we need the animation on multiple targets?
        if (isAnyOf(_impl->skill.targetType(), Multiple, Aura, AuraIncludingSelf))
        {
            std::vector<Ref<Actor const>> targets;
            for (Actor const & target : _impl->targets[_impl->skill.targetType()])
            {
                // Filter targets here
                if (isValidTarget(performer(), _impl->skill.targetNature(), target, _impl->forceAttack))
                    targets.emplace_back(target);
            }

            World::broadcastAround(performer(),
                                   SC::SkillSetTargetsPacket{performer(), _impl->skill, targets}, true);
        }
    }

    // TODO: ensure targets are still valid at every update

    setFinished((_impl->castingElapsed += elapsed) >= _impl->castingDuration);
}

void SkillAction::onFinished()
{
    if (_impl->castingStarted)
    {
        if (_impl->skill.operatingType() == Toggle)
        {
            Actor & self           = _impl->targets[Self].front();
            auto const matchToggle = [this] (auto const & e) { return e->skillUid() == _impl->skill.uid(); };

            if (std::ranges::any_of(self.effects(), matchToggle))
                cancelCurrentEffects();
            else
                applyEffects();
        }
        else
        {
            cancelCurrentEffects();
            applyEffects();
        }
    }

    performer().state = _impl->previousState;
}

void SkillAction::onCanceled()
{
    if (_impl->castingStarted)
    {
        World::broadcastAround(performer(), SC::SkillCancelPacket{performer()}, true);

        SC::ChatSystemSayPacket msg{SystemMessageId::_1sCastingHasBeenInterrupted};
        msg.appendName(_impl->skill);
        World::send(performer(), std::move(msg));
    }

    performer().state = _impl->previousState;
}

void SkillAction::sendUseSkillSystemMessage() const
{
    SC::ChatSystemSayPacket msg{SystemMessageId::Use_1};
    msg.appendArg(SysMsgArg::SkillName{_impl->skill.uid()});
    World::send(performer(), std::move(msg));
}

void SkillAction::selectTargets(EffectTargetType const targetType, SkillTargetNature const targetNature)
{
    // For now, we consider that all effects use the same range, so no need to scan the area again for the same type
    if (_impl->targets.contains(targetType))
        return;

    OptRef<Actor> target;
    switch (targetType)
    {
        case Self:
            _impl->targets[targetType].emplace_back(performer());
            return;

        case Single:
        {
            target = _impl->targetId == performer().id() ? performer() : *World::actor(*_impl->targetId);
            if (isValidTarget(performer(), targetNature, target, _impl->forceAttack))
                _impl->targets[targetType].emplace_back(target);

            return;
        }

        case Multiple:
            target = _impl->targetId == performer().id() ? performer() : *World::actor(*_impl->targetId);
            _impl->targets[targetType].emplace_back(target);
            break;

        case AuraIncludingSelf:
            _impl->targets[targetType].emplace_back(performer());
            [[fallthrough]];

        case Aura:
            target = performer();
            break;
    }

    World::forEachActorAround(target, [&] (auto & actor)
    {
        if (isValidTarget(performer(), targetNature, actor, _impl->forceAttack))
            _impl->targets[targetType].emplace_back(actor);
    });
}

void SkillAction::adjustCastingDuration()
{
    if (_impl->skill.isMagic())
    {
        auto const mAtkSpeedRatio = performer().stats()[StatId::MAtkSpeed] / 333;
        _impl->castingDuration    = Utils::Chrono::Clock::toDuration(_impl->skill.castDuration() / mAtkSpeedRatio);

        if (_impl->skill.castDuration() > 550ms && _impl->castingDuration <= 550ms) // Avoid broken animations
            _impl->castingDuration = 550ms;
    }
    else
    {
        auto const pAtkSpeedRatio = performer().stats()[StatId::PAtkSpeed] / 300;
        _impl->castingDuration    = Utils::Chrono::Clock::toDuration(_impl->skill.castDuration() / pAtkSpeedRatio);

        if (_impl->skill.castDuration() > 500ms && _impl->castingDuration < 500ms) // Avoid broken animations
            _impl->castingDuration = 500ms;
    }

    // Skill targets must be sent 350ms (including latency) before the end of the animation to display correctly
    _impl->notifyTargetsTrigger = std::max(_impl->castingDuration - 350ms, ClockDuration::zero());
}

void SkillAction::cancelCurrentEffects() const
{
    forEachTarget([this] (auto & target) mutable
    {
        auto v = target.effects() | std::views::filter([this] (auto const & e) {
            return e->skillUid() == _impl->skill.uid();
        });
        std::ranges::for_each(v, [] (auto const & effect) { return effect->cancel(); });
    });
}

void SkillAction::applyEffects() const
{
    // Each effect has its own set of valid targets
    for (auto const & effect : _impl->skill.effects())
    {
        for (Actor & target : _impl->targets.at(effect->targetType()))
            effect->apply(performer(), target);
    }
}

void SkillAction::forEachTarget(std::function<void(Actor &)> const & f) const
{
    if (f)
    {
        for (auto & targets : _impl->targets | std::views::values)
            for (auto & target : targets)
                f(target);
    }
}
