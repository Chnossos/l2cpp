/// @author    Chnossos
/// @date      Created on 2026-05-09

#pragma once

// Project includes
#include <l2cpp/Typedefs.hpp>

// C++ includes
#include <chrono>
#include <unordered_map>

using namespace std::literals;

using ClockDuration  = std::chrono::steady_clock::duration;
using ClockTimePoint = std::chrono::steady_clock::time_point;

using GameObjectId = u32;

using ItemId     = s64;
using SkillId    = u16;
using SkillLevel = u16;
using StatValue  = double;

using DamageDealtTable = std::unordered_map<GameObjectId, StatValue>;
