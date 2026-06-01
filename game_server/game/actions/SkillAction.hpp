/// @author    Chnossos
/// @date      Created on 2026-04-08

#pragma once

// Project includes
#include "../constants/EffectTargetType.hpp"
#include "../constants/SkillTargetNature.hpp"
#include "Action.hpp"

#include <l2cpp/Pimpl.hpp>

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
    struct SkillActionImpl;
    Pimpl<SkillActionImpl> _impl;
};
