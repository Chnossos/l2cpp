/// @author    Chnossos
/// @date      Created on 2026-03-16

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/constants/StatId.hpp>
#include <gs/game/ecs/Component.hpp>

// C++ includes
#include <array>

class Actor;

struct Stats : public Component
{
    Stats();

    auto operator[](size_t const index)       -> StatValue & { return _stats[index];                     }
    auto operator[](size_t const index) const -> StatValue   { return _stats[index];                     }
    auto operator[](StatId const index)       -> StatValue & { return _stats[std::to_underlying(index)]; }
    auto operator[](StatId const index) const -> StatValue   { return _stats[std::to_underlying(index)]; }

    void compute(Actor const &);

    void regenHpFully();
    void regenMpFully();
    void regenCpFully();
    void regenFully();

private:
    void reset(Actor const &);
    void computeSkillEffects(Actor const &);

private:
    std::array<StatValue, std::to_underlying(StatId::Count)> _stats{};
};
