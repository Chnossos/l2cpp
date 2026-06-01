/// @author    Chnossos
/// @date      Created on 2026-05-22

#pragma once

// Project includes
#include <gs/game/actions/Action.hpp>
#include <gs/game/constants/SocialActionId.hpp>

class SocialAction : public Action
{
public:
    SocialAction(Actor & performer, SocialActionId);

public:
    bool canBeInterruptedByAnotherAction() const override { return true; }

private:
    void onStarted() override;

private:
    SocialActionId _actionId;
};
