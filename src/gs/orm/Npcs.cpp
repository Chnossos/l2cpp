/// @author    Chnossos
/// @date      Created on 2026-06-07

#include "Npcs.hpp"

// Project includes
#include <common/services/Database.hpp>
#include <gs/game/directories/NpcDirectory.hpp>
#include <gs/game/spawn/SpawnManager.hpp>

// C++ includes
#include <ranges>

using namespace Utils::Sql;

void Orm::loadNpcs()
{
    SQLite::Statement query{Database::instance(), R"(SELECT * FROM npc_templates)"};
    while (query.executeStep())
    {
        NpcInfo npc;
        extract                      (query, "id",               npc.id);
        extract                      (query, "attackable",       npc.attackable);
        extract                      (query, "collision_height", npc.collisionHeight);
        extract                      (query, "collision_radius", npc.collisionRadius);
        extract                      (query, "level",            npc.level);
        extract                      (query, "xp",               npc.xp);
        extract                      (query, "xp_rate",          npc.xpRate);
        extract                      (query, "sp",               npc.sp);
        extract                      (query, "max_hp",           npc.maxHp);
        extract                      (query, "max_mp",           npc.maxMp);
        extract                      (query, "hp_regen",         npc.hpRegen);
        extract                      (query, "mp_regen",         npc.mpRegen);
        extract<std::chrono::seconds>(query, "corpse_duration",  npc.corpseDuration);
        npc.type = npc.attackable ? ActorType::Monster : ActorType::Npc;

        auto const id = npc.id;
        (npc.attackable ? NpcDirectory::_monsters : NpcDirectory::_npcs).try_emplace(id, std::move(npc));
    }

    NpcDirectory::_maxLevel = Database::instance().execAndGet("SELECT MAX(level) FROM npc_templates").getUInt();
}

template<std::integral T>
static auto to(std::string_view str)
{
    T value;

    auto const result = std::from_chars(str.data(), str.data() + str.size(), value);
    L2CPP_B_ASSERT(result.ec != std::errc::invalid_argument,
                   "Failed to convert '{}' to type '{}'", str, typeid(T).name());

    return value;
}

void Orm::loadSpawnAreas()
{
    SQLite::Statement query{Database::instance(), R"(SELECT * FROM npc_spawn_areas)"};
    while (query.executeStep())
    {
        auto & area = sSpawnManager._spawnAreas.emplace_back();
        extract(query, "tag",   area.name);
        extract(query, "min_z", area.minZ);
        extract(query, "max_z", area.maxZ);

        std::string rawVertices;
        extract(query, "vertices", rawVertices);
        for (auto const xy : std::views::split(rawVertices, '|'))
        {
            // TODO: parsing error handling
            auto const str = std::string_view{xy};
            auto const pos = str.find(';');
            auto const x   = str.substr(0, pos);
            auto const y   = str.substr(pos + 1);
            area.vertices.emplace_back(to<s32>(x), to<s32>(y));
        }
    }
}

void Orm::loadNpcMakers()
{
    SQLite::Statement query{Database::instance(), R"(
        SELECT
            *
        FROM
            npc_npc_makers
        WHERE
            'event_name' NOT IN spawn_conditions
    )"};

    while (query.executeStep())
    {
        auto   name  = query.getColumn("name").getString();
        auto & maker = sSpawnManager._npcMakers.try_emplace(name).first->second;
        maker.name   = std::move(name);

        std::string areaSpan;
        extract(query, "area_span", areaSpan);
        for (auto const area : std::views::split(areaSpan, '&'))
            maker.areaSpan.emplace_back(std::string_view{area});

        std::string areaExclusions;
        extract(query, "area_exclusions", areaExclusions);
        for (auto const area : std::views::split(areaExclusions, '|'))
            maker.areaExclusions.emplace_back(std::string_view{area});

        extract(query, "max_npc_count", maker.maxNpcCount);

        // TODO: custom_behavior + parameters
    }

    loadFixedNpcMakers();
    loadRandomNpcMakers();
}

void Orm::loadFixedNpcMakers()
{
    SQLite::Statement query{Database::instance(), R"(
        SELECT * FROM npc_spawn_maker_fixed_positions
    )"};

    while (query.executeStep())
    {
        auto const makerTag = query.getColumn("maker_tag").getString();
        auto const makerIt  = sSpawnManager._npcMakers.find(makerTag);
        if (makerIt == sSpawnManager._npcMakers.end())
            continue;

        auto & entry = makerIt->second.npcComposition.emplace_back();
        extract(query, "npc_template_id", entry.npcTemplateId);

        auto & pos = entry.position.emplace();
        extract(query, "pos_x",       pos.x);
        extract(query, "pos_y",       pos.y);
        extract(query, "pos_z",       pos.z);
        extract(query, "orientation", pos.orientation);

        extract<std::chrono::minutes>(query, "respawn_duration", entry.respawnDuration);
        extractOr(query, "respawn_window", entry.respawnWindow, ClockDuration{0min}); // FIXME: cast

        entry.npcCount = 1;
    }
}

void Orm::loadRandomNpcMakers()
{
    SQLite::Statement query{Database::instance(), R"(
        SELECT * from npc_spawn_maker_random_positions
    )"};

    while (query.executeStep())
    {
        auto const makerTag = query.getColumn("maker_tag").getString();
        auto const makerIt  = sSpawnManager._npcMakers.find(makerTag);
        if (makerIt == sSpawnManager._npcMakers.end())
            continue;

        auto & entry = makerIt->second.npcComposition.emplace_back();
        extract(query, "npc_template_id", entry.npcTemplateId);
        extract(query, "total",           entry.npcCount);

        extract<std::chrono::minutes>(query, "respawn_duration", entry.respawnDuration);
        extractOr(query, "respawn_window", entry.respawnWindow, ClockDuration{0min}); // FIXME: cast
    }
}

