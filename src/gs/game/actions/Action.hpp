/// @author    Chnossos
/// @date      Created on 2026-03-20

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../constants/ActionType.hpp"

class Actor;

class Action
{
public:
    Action(ActionType type, Actor & performer) noexcept;
    Action(Action &&) noexcept = default;
    Action & operator=(Action &&) noexcept = default;
    virtual ~Action() noexcept;

public:
    auto type()           const -> ActionType;
    auto performer()      const -> Actor &;
    auto isFinished()     const -> bool;
    auto startTime()      const -> ClockTimePoint;
    auto lastUpdateTime() const -> ClockTimePoint;

public:
    virtual bool canBeInterruptedByAnotherAction() const = 0;

public:
    void update(ClockDuration elapsed);
    void cancel();

protected:
    void setFinished(bool finished);

private:
    virtual void onStarted()               {}
    virtual void updateImpl(ClockDuration) {}
    virtual void onFinished()              {}
    virtual void onCanceled()              {}

private:
    ActionType     _type;
    Ref<Actor>     _performer;
    bool           _finished;
    ClockTimePoint _startTime, _lastUpdateTime;
};
