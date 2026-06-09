/// @author    Chnossos
/// @date      Created on 2026-06-08

#pragma once

// Project includes
#include <gs/game/components/Position.hpp>

// C++ includes
#include <vector>

namespace Orm
{
    void loadSpawnPoints();
}

struct SpawnPoint
{
    u32                          npcTemplateId;
    Position                     position;
    std::optional<u32>           chancePercent;
    std::optional<ClockDuration> respawnDuration;
    std::optional<ClockDuration> respawnWindow;
    std::optional<GameObjectId>  spawnedActorId;
};

class SpawnManager
{
    friend void Orm::loadSpawnPoints();

private:
    SpawnManager() noexcept = default;

public:
    static auto instance() -> SpawnManager &;

public:
    auto count() const -> size_t;

public:
    void load();
    void unload();
    void spawnActorsAround(Position const &);

private:
    std::vector<SpawnPoint> _spawnPoints;
};

#define sSpawnManager SpawnManager::instance()
