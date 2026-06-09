/// @author    Chnossos
/// @date      Created on 2026-06-08

#pragma once

// Project includes
#include <gs/game/components/Position.hpp>

// C++ includes
#include <unordered_set>
#include <vector>

namespace Orm
{
    void loadSpawnPoints();
}

struct SpawnPoint
{
    std::vector<std::pair<Position, u8>> possiblePositions;
    u32                                  npcTemplateId   = 0;
    ClockDuration                        respawnDuration = ClockDuration::zero();
    ClockDuration                        respawnWindow   = ClockDuration::zero();
};

class SpawnManager
{
    friend void Orm::loadSpawnPoints();

private:
    SpawnManager() noexcept = default;

public:
    static auto instance() -> SpawnManager &;

public:
    auto loadCount() const -> size_t;
    auto spawnedCount() const -> size_t;

public:
    void load();
    void unload();

private:
    std::unordered_multimap<u32, SpawnPoint> _spawnPoints;
    std::unordered_set<GameObjectId>         _spawnedActorIds;
};

#define sSpawnManager SpawnManager::instance()
