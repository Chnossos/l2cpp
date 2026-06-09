/// @author    Chnossos
/// @date      Created on 2026-06-08

#include "SpawnManager.hpp"

#include <gs/game/World.hpp>
#include <gs/game/actor/Npc.hpp>
#include <gs/utils/Maths.hpp>
#include <spdlog/spdlog.h>

auto SpawnManager::instance() -> SpawnManager &
{
    static SpawnManager instance;
    return instance;
}

auto SpawnManager::count() const -> size_t
{
    return _spawnPoints.size();
}

void SpawnManager::load()
{
    Orm::loadSpawnPoints();
}

void SpawnManager::unload()
{
    _spawnPoints.clear();
}

void SpawnManager::spawnActorsAround(Position const & origin)
{
    for (auto & point : _spawnPoints)
    {
        if (Utils::Maths::distance(origin, point.position) > 6000)
            continue;

        auto const npc = World::addNpc(point.npcTemplateId);
        npc->setPosition(point.position);
        point.spawnedActorId = npc->id();

        SPDLOG_TRACE("Spawned '{}'", point.npcTemplateId);
    }
}
