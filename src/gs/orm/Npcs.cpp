/// @author    Chnossos
/// @date      Created on 2026-06-07

#include "Npcs.hpp"

// Project includes
#include <common/services/Database.hpp>
#include <gs/game/directories/NpcDirectory.hpp>

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
