/// @author    Chnossos
/// @date      Created on 2026-06-07

#include "Npcs.hpp"

// Project includes
#include <common/services/Database.hpp>
#include <gs/game/directories/NpcDirectory.hpp>
#include <gs/game/spawn/SpawnManager.hpp>

using namespace Utils::Sql;

void Orm::loadNpcs()
{
    SQLite::Statement query{Database::instance(), R"(SELECT * FROM npc_templates)"};
    while (query.executeStep())
    {
        NpcInfo npc;
        extract(query, "id",               npc.id);
        extract(query, "attackable",       npc.attackable);
        extract(query, "collision_height", npc.collisionHeight);
        extract(query, "collision_radius", npc.collisionRadius);
        extract(query, "level",            npc.level);
        extract(query, "xp",               npc.xp);
        extract(query, "xp_rate",          npc.xpRate);
        extract(query, "sp",               npc.sp);
        extract(query, "max_hp",           npc.maxHp);
        extract(query, "max_mp",           npc.maxMp);
        extract(query, "hp_regen",         npc.hpRegen);
        extract(query, "mp_regen",         npc.mpRegen);
        npc.type = npc.attackable ? ActorType::Monster : ActorType::Npc;

        auto const id = npc.id;
        (npc.attackable ? NpcDirectory::_monsters : NpcDirectory::_npcs).try_emplace(id, std::move(npc));
    }

    NpcDirectory::_maxLevel = Database::instance().execAndGet("SELECT MAX(level) FROM npc_templates").getUInt();
}

void Orm::loadSpawnPoints()
{
    SQLite::Statement query{Database::instance(), R"(SELECT * FROM npc_spawn_points)"};
    while (query.executeStep())
    {
        SpawnPoint p{.npcTemplateId = query.getColumn("npc_template_id").getUInt()};

        auto & [pos, chance] = p.possiblePositions.emplace_back();
        extract  (query, "pos_x",            pos.x);
        extract  (query, "pos_y",            pos.y);
        extract  (query, "pos_z",            pos.z);
        extract  (query, "orientation",      pos.orientation);
        extractOr(query, "chance_percent",   chance,            100);
        extractOr(query, "respawn_duration", p.respawnDuration, 0s);
        extractOr(query, "respawn_window",   p.respawnWindow,   0s);

        auto const npcId = p.npcTemplateId;
        sSpawnManager._spawnPoints.emplace(npcId, std::move(p));
    }

    query = SQLite::Statement{Database::instance(), R"(SELECT * FROM npc_spawn_territory_edges)"};
    while (query.executeStep())
    {
        auto const tag          = query.getColumn("territory_tag").getString();
        auto & [name, edges, _] = sSpawnManager._spawnTerritories[tag];

        if (name.empty())
            name = tag;

        auto & [x, y, minZ, maxZ] = edges.emplace_back();
        extract(query, "pos_x", x);
        extract(query, "pos_y", y);
        extract(query, "min_z", minZ);
        extract(query, "max_z", maxZ);
    }

    query = SQLite::Statement{Database::instance(), R"(SELECT * FROM npc_spawn_swarms)"};
    while (query.executeStep())
    {
        auto swarmName                           = query.getColumn("name").getString();
        auto & [_, name, territory, maxNpcCount] = sSpawnManager._swarms[swarmName];
        name                                     = std::move(swarmName);

        extract(query, "territory_tag", territory);
        extract(query, "max_npc_count", maxNpcCount);
    }

    query = SQLite::Statement{Database::instance(), R"(SELECT * FROM npc_spawn_swarm_compositions)"};
    while (query.executeStep())
    {
        auto const templateId = query.getColumn("npc_template_id").getUInt();
        auto const swarmName  = query.getColumn("swarm_name"     ).getString();

        SwarmEntityInfo info{.npcTemplateId = templateId};
        extract                      (query, "npc_count",        info.npcCount);
        extract<std::chrono::seconds>(query, "respawn_duration", info.respawnDuration);
        extract<std::chrono::seconds>(query, "respawn_window",   info.respawnWindow);
        sSpawnManager._swarms[swarmName].composition.try_emplace(templateId, std::move(info));
    }
}
