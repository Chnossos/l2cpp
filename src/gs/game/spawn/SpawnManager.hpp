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
    using Chance = u8;
    std::vector<std::pair<Position, Chance>> possiblePositions;
    NpcTemplateId                            npcTemplateId   = 0;
    ClockDuration                            respawnDuration = ClockDuration::zero();
    ClockDuration                            respawnWindow   = ClockDuration::zero();
};

struct SwarmEntityInfo
{
    NpcTemplateId npcTemplateId   = 0;
    ClockDuration respawnDuration = ClockDuration::zero();
    ClockDuration respawnWindow   = ClockDuration::zero();
    u16           npcCount        = 0;
};

struct SwarmInfo
{
    std::unordered_map<NpcTemplateId, SwarmEntityInfo> composition;
    std::string                                        name, territoryTag;
    u16                                                maxNpcCount{};
};

struct SpawnTerritoryEdge
{
    s32 x    = 0, y    = 0;
    s32 minZ = 0, maxZ = 0;
};

struct SpawnTerritory
{
    std::string                     name;
    std::vector<SpawnTerritoryEdge> edges;
    std::vector<Position>           spawnPoints;
};

class SpawnManager
{
    friend void Orm::loadSpawnPoints();

private:
    SpawnManager() noexcept = default;

public:
    static auto instance() -> SpawnManager &;

public:
    auto loadCount()    const -> size_t;
    auto spawnedCount() const -> size_t;

public:
    void load();
    void unload();

private:
    std::unordered_multimap<NpcTemplateId, SpawnPoint> _spawnPoints;
    std::unordered_set<GameObjectId>                   _spawnedActorIds;
    std::unordered_map<std::string, SpawnTerritory>    _spawnTerritories;
    std::unordered_map<std::string, SwarmInfo>         _swarms;
};

#define sSpawnManager SpawnManager::instance()
