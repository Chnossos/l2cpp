/// @author    Chnossos
/// @date      Created on 2026-03-24

#include "Action.hpp"

#include "../actor/Actor.hpp"

Action::Action(ActionType const type, Actor & performer) noexcept
    : _type(type)
    , _performer(performer)
    , _finished(false)
    , _lastUpdateTime(ClockTimePoint::min())
{}

Action::~Action() noexcept = default;

auto Action::type()           const -> ActionType     { return _type;           }
auto Action::performer()      const -> Actor &        { return _performer;      }
auto Action::isFinished()     const -> bool           { return _finished;       }
auto Action::startTime()      const -> ClockTimePoint { return _startTime;      }
auto Action::lastUpdateTime() const -> ClockTimePoint { return _lastUpdateTime; }

void Action::update(ClockDuration const elapsed)
{
    if (_lastUpdateTime == ClockTimePoint::min()) [[unlikely]]
    {
        _startTime = _lastUpdateTime = std::chrono::steady_clock::now();
        onStarted();
    }
    else
    {
        _lastUpdateTime += elapsed;
        updateImpl(elapsed);
    }

    if (_finished)
        onFinished();
}

void Action::cancel()
{
    // This function can be called from two places:
    // 1) From actor.cancelAction();
    // 2) From actor.currentAction()->cancel();
    //
    // We need to ensure both ways will call Action::cancel() and remove the actions from the actor.
    // To avoid an infinite recursion loop, Actor::cancelAction will first release its currentAction pointer
    // so that potential next call will do nothing.

    if (performer().currentAction())
        performer().cancelAction();
    else
        onCanceled();
}

void Action::setFinished(bool const finished)
{
    _finished = finished;
}
