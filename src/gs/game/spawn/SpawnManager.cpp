/// @author    Chnossos
/// @date      Created on 2026-06-08

#include "SpawnManager.hpp"

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actor/Npc.hpp>

// C++ includes
#include <ranges>
#include <set>

// Third-party includes
#include <PoissonGenerator/PoissonGenerator.h>
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

// static bool point_within_polygon(vec2 *pt, vec2 *pt_list, unsigned short n_pt)
// {
//     short i, j;
//     short pos = 0, neg = 0;
//     short x, y, x1, y1, x2, y2, d;
//     //Check if a triangle or higher n-gon
//     if (n_pt < 3)
//     {
//         // printf("point_within_polygon(), n_pt < 3 !\n");
//         return false;
//     }
//
//     //n>2 Keep track of cross product sign changes
//
//     for (i = 0; i < n_pt; i++)
//     {
//         //If point is in the polygon
//         if (pt_list[i].x == pt->x && pt_list[i].y == pt->y)
//             return true;
//
//         //Form a segment between the i'th point
//         x1 = pt_list[i].x;
//         y1 = pt_list[i].y;
//
//         //And the i+1'th, or if i is the last, with the first point
//         j = i < (n_pt - 1) ? i + 1 : 0;
//
//         x2 = pt_list[j].x;
//         y2 = pt_list[j].y;
//
//         x = pt->x;
//         y = pt->y;
//
//         //Compute the cross product
//         d = (x - x1)*(y2 - y1) - (y - y1)*(x2 - x1);
//
//         if (d > 0) pos++;
//         if (d < 0) neg++;
//
//         //If the sign changes, then point is outside
//         if (pos > 0 && neg > 0)
//             return false;
//     }
//
//     //If no change in direction, then on same side of all segments, and thus inside
//     return true;
// }

struct BoundingBox { std::pair<s32, s32> topLeft, topRight, bottomLeft, bottomRight; };

static auto makeBoundingBox(std::span<SpawnTerritoryEdge const> const points) -> BoundingBox
{
    std::set<s32> x, y;

    for (auto const & point : points)
    {
        x.emplace(point.x);
        y.emplace(point.y);
    }

    return {
        .topLeft     = {*x.cbegin(),  *y.crbegin()},
        .topRight    = {*x.crbegin(), *y.crbegin()},
        .bottomLeft  = {*x.cbegin(),  *y.cbegin() },
        .bottomRight = {*x.crbegin(), *y.cbegin() },
    };
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

    for (auto const & swarm : _swarms | std::views::values)
    {
        auto & territory = _spawnTerritories[swarm.territoryTag];
        if (swarm.maxNpcCount <= territory.spawnPoints.size())
            continue;

        auto const boundingBox = makeBoundingBox(territory.edges);
        auto const xOffset     = boundingBox.topLeft.first;
        auto const yOffset     = boundingBox.topLeft.second;
        auto const width       = boundingBox.topRight  .first  - xOffset;
        auto const height      = boundingBox.bottomLeft.second - yOffset;

        for (auto & p : PoissonGenerator::generateVogelPoints(swarm.maxNpcCount))
        {
            p.x = xOffset + p.x * width;
            p.y = yOffset + p.y * height;
            territory.spawnPoints.emplace_back(static_cast<s32>(p.x), static_cast<s32>(p.y),
                                               territory.edges.front().minZ/*, TODO: random orientation*/);
        }

        size_t i = 0;
        for (auto const & [npcTemplateId, info] : swarm.composition)
        {
            for (size_t j = 0; j < info.npcCount && i < territory.spawnPoints.size(); ++j)
                World::addNpc(npcTemplateId, territory.spawnPoints[i++]);
        }
    }
}

void SpawnManager::unload()
{
    _spawnPoints.clear();
}
