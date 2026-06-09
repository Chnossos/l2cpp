/// @author    Chnossos
/// @date      Created on 2026-06-07

#include "Npcs.hpp"

// Project includes
#include <common/services/Database.hpp>
#include <gs/game/directories/NpcDirectory.hpp>
#include <gs/game/spawn/SpawnManager.hpp>

void Orm::loadNpcs()
{
    SQLite::Statement query{Database::instance(), R"(SELECT * FROM npc_templates)"};
    while (query.executeStep())
    {
        auto const id         = query.getColumn("id"        ).getUInt();
        bool const attackable = query.getColumn("attackable").getUInt();

        auto & container    = attackable ? NpcDirectory::_monsters : NpcDirectory::_npcs;
        auto & npc          = container[id];
        npc.id              = id;
        npc.attackable      = attackable;
        npc.type            = attackable ? ActorType::Monster : ActorType::Npc;
        npc.collisionHeight =                 query.getColumn("collision_height").getDouble();
        npc.collisionRadius =                 query.getColumn("collision_radius").getDouble();
        npc.level           = static_cast<u8>(query.getColumn("level"           ).getUInt());
        npc.xp              =                 query.getColumn("xp"              ).getUInt();
        npc.xpRate          =                 query.getColumn("xp_rate"         ).getDouble();
        npc.sp              =                 query.getColumn("sp"              ).getUInt();
        npc.maxHp           =                 query.getColumn("max_hp"          ).getDouble();
        npc.maxMp           =                 query.getColumn("max_mp"          ).getDouble();
        npc.hpRegen         =                 query.getColumn("hp_regen"        ).getDouble();
        npc.mpRegen         =                 query.getColumn("mp_regen"        ).getDouble();
    }

    NpcDirectory::_maxLevel = Database::instance().execAndGet("SELECT MAX(level) FROM npc_templates").getUInt();
}

void Orm::loadSpawnPoints()
{
    SQLite::Statement query{Database::instance(), R"(SELECT * FROM npc_spawn_points)"};
    while (query.executeStep())
    {
        auto & p = sSpawnManager._spawnPoints.emplace_back();
        p.npcTemplateId        =                  query.getColumn("npc_template_id").getUInt();
        p.position.x           =                  query.getColumn("pos_x"          ).getInt();
        p.position.y           =                  query.getColumn("pos_y"          ).getInt();
        p.position.z           =                  query.getColumn("pos_z"          ).getInt();
        p.position.orientation = static_cast<u16>(query.getColumn("orientation"    ).getUInt());

        if (auto const chanceCol = query.getColumn("chance_percent"); !chanceCol.isNull())
            p.chancePercent = chanceCol.getUInt();

        if (auto const respawnDurationCol = query.getColumn("respawn_duration"); !respawnDurationCol.isNull())
            p.respawnDuration = ClockDuration{respawnDurationCol.getUInt()};

        if (auto const respawnWindowCol = query.getColumn("respawn_window"); !respawnWindowCol.isNull())
            p.respawnWindow = ClockDuration{respawnWindowCol.getUInt()};
    }
}
