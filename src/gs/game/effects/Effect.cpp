/// @author    Chnossos
/// @date      Created on 2026-04-14

#include <gs/game/effects/Effect.hpp>

// Project includes
#include <gs/game/actor/Character.hpp>
#include <gs/game/skill/Skill.hpp>
#include <gs/utils/Chrono.hpp>

Effect::Effect(
    EffectType      const   type
    , Actor               & source
    , Actor               & target
    , SkillUid      const   skillUid
    , ClockDuration const   effectDuration
    , ClockDuration const   tickDuration
    , ClockDuration const   initialTriggerDuration
)
    : _type{type}
    , _source{source}
    , _target{target}
    , _skillUid{skillUid}
    , _duration{effectDuration}
    , _elapsed{ClockDuration::zero()}
    , _elapsedSinceLastTick{ClockDuration::zero()}
    , _tickDuration{tickDuration}
    , _initialTriggerDuration{initialTriggerDuration}
    , _finished{false}
{}

Effect::~Effect() = default;

auto Effect::type()     const -> EffectType    { return _type;     }
auto Effect::skillUid() const -> SkillUid      { return _skillUid; }
auto Effect::duration() const -> ClockDuration { return _duration; }
auto Effect::elapsed()  const -> ClockDuration { return _elapsed;  }

auto Effect::remainingDuration() const -> ClockDuration
{
    return _duration > ClockDuration::zero() ? _duration - _elapsed : _duration;
}

auto Effect::source() const -> Actor & { return _source; }
auto Effect::target() const -> Actor & { return _target; }

bool Effect::update(ClockDuration const elapsed)
{
    if (_elapsed == ClockDuration::zero())
    {
        onStarted();

        if (_initialTriggerDuration == ClockDuration::zero())
            onTick();

        if (_duration > ClockDuration::zero())
            _elapsed  = ClockDuration(1); // ensure we don't step inside this block again by making _elapsed non-zero
    }

    /**/ if (Utils::Chrono::thresholdCrossed(_elapsed, elapsed, _duration))
    {
        _finished = true;
    }
    else if (Utils::Chrono::thresholdCrossed(_elapsed, elapsed, _initialTriggerDuration))
    {
        onTick();
        _elapsedSinceLastTick = _elapsed + elapsed - _initialTriggerDuration;
    }
    else if (Utils::Chrono::thresholdCrossed(_elapsedSinceLastTick, elapsed, _tickDuration))
    {
        onTick();
        _elapsedSinceLastTick += elapsed - _tickDuration;
    }
    else
        _elapsedSinceLastTick += elapsed;

    if (_finished)
        onFinished();
    else
        _elapsed += elapsed;

    return _finished;
}

void Effect::cancel()
{
    _finished = true;
}
