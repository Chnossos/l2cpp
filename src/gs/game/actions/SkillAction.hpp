/// @author    Chnossos
/// @date      Created on 2026-04-08

#pragma once

// Project includes
#include <common/Pimpl.hpp>
#include <gs/game/actions/Action.hpp>
#include <gs/game/constants/EffectTargetType.hpp>
#include <gs/game/constants/SkillTargetNature.hpp>

// C++ includes
#include <functional>

class SkillTemplate;

class SkillAction final : public Action
{
public:
    explicit SkillAction(Actor & performer, SkillTemplate const & skill, bool forceAttack);
    SkillAction(SkillAction &&) noexcept;
    SkillAction & operator=(SkillAction &&) noexcept;
    ~SkillAction() override;

public:
    bool canBeInterruptedByAnotherAction() const override;

private:
    void onStarted() override;
    void updateImpl(ClockDuration) override;
    void onFinished() override;
    void onCanceled() override;
    void sendUseSkillSystemMessage() const;
    void selectTargets(EffectTargetType, SkillTargetNature);
    void adjustCastingDuration();
    void cancelCurrentEffects() const;
    void applyEffects() const;
    void forEachTarget(std::function<void(Actor &)> const &) const;

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
