/// @author    Chnossos
/// @date      Created on 2026-03-20

#pragma once

// Project includes
#include "Action.hpp"
#include "../components/Position.hpp"
#include "../constants/ActorState.hpp"

class MoveAction final : public Action
{
public:
    enum class Input { Keyboard, Mouse };

public:
    MoveAction(Actor & performer, Position const & origin, Position const & target, Input input);

public:
    bool canBeInterruptedByAnotherAction() const override;

private:
    void onStarted() override;
    void updateImpl(ClockDuration) override;
    void onFinished() override;
    void onCanceled() override;

    Position   _origin, _target;
    Input      _input;
    float      _currentDistance = 0, _totalDistance = 0;
    ActorState _previousState;
};
