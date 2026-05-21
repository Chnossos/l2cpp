/// @author    Chnossos
/// @date      Created on 2026-04-08

#pragma once

// Project includes
#include "Action.hpp"

#include <l2cpp/Pimpl.hpp>

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
    void selectTargets();
    void sendUseSystemMessage() const;

private:
    struct SkillActionImpl;
    Pimpl<SkillActionImpl> _impl;
};
