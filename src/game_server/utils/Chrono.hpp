/// @author    Chnossos
/// @date      Created on 2026-03-25

#pragma once

// Project includes
#include "../Typedefs.hpp"

namespace Utils::Chrono
{
    class Clock
    {
        using UnderlyingClock = std::chrono::steady_clock;

    public:
        Clock() noexcept
            : start(UnderlyingClock::now())
        {}

    public:
        template<typename Rep, typename Period>
        static constexpr auto toDuration(std::chrono::duration<Rep, Period> d) -> UnderlyingClock::duration
        {
            return std::chrono::duration_cast<UnderlyingClock::duration>(d);
        }

    public:
        auto restart() -> UnderlyingClock::duration
        {
            auto const now = UnderlyingClock::now();
            return now - std::exchange(start, now);
        }

    private:
        std::chrono::steady_clock::time_point start;
    };

    constexpr bool thresholdCrossed(ClockDuration const totalElapsed,
                                    ClockDuration const elapsed,
                                    ClockDuration const elapsedThreshold)
    {
        return totalElapsed <= elapsedThreshold && totalElapsed + elapsed >= elapsedThreshold;
    }

    constexpr bool thresholdCrossed(ClockTimePoint const currentTimePoint,
                                    ClockDuration  const elapsed,
                                    ClockTimePoint const thresholdTimePoint)
    {
        return currentTimePoint <= thresholdTimePoint && currentTimePoint + elapsed >= thresholdTimePoint;
    }
}
