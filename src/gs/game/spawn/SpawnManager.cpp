/// @author    Chnossos
/// @date      Created on 2026-06-08

#include "SpawnManager.hpp"

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actor/Npc.hpp>

// C++ includes
#include <ranges>

// Third-party includes
#include <spdlog/spdlog.h>

auto SpawnManager::instance() -> SpawnManager &
{
    static SpawnManager instance;
    return instance;
}

auto SpawnManager::loadCount() const -> size_t
{
    return _spawnPoints.size();
}

auto SpawnManager::spawnedCount() const -> size_t
{
    return _spawnedActorIds.size();
}

void SpawnManager::load()
{
    Orm::loadSpawnPoints();

    for (auto const & spawnPoint : _spawnPoints | std::views::values)
    {
        if (auto const npc = World::addNpc(spawnPoint.npcTemplateId, spawnPoint.possiblePositions.front().first))
            _spawnedActorIds.emplace(npc->id());
        else
            SPDLOG_WARN("Failed to spawn NPC (template id: {}): id not found", spawnPoint.npcTemplateId);
    }
}

void SpawnManager::unload()
{
    _spawnPoints.clear();
}
