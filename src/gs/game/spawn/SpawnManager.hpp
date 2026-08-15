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
    void loadSpawnAreas();
    void loadNpcMakers();
    void loadFixedNpcMakers();
    void loadRandomNpcMakers();
}

enum class SpawnBehavior
{
    DefaultMaker,
    DefaultUseDbMaker,
    ExclusiveSpawnNormal,
    MakerInstantSpawn,
    MakerInstantSpawnRandom,
    MakerInstantSpawnSerial,
    ManageTeleportDungeon,
    OnDayNightSpawn,
    RandomSpawn,
    RoyalReqNextMaker,
    RoyalRushMaker,
    RoyalSpawnTreasureBox,
    UniqueNpcKillEvent,
};

/// Model the @code npcpos:npc@endcode and @code npcpos:npc_ex@endcode entries.
struct NpcMakerEntry
{
    std::optional<Position> position;
    NpcTemplateId           npcTemplateId   = 0;
    ClockDuration           respawnDuration = ClockDuration::zero();
    ClockDuration           respawnWindow   = ClockDuration::zero();
    u16                     npcCount        = 0;
};

/// Model the @code npcpos:npc_maker@endcode entries.
struct NpcMaker
{
    std::string                name;
    std::vector<std::string>   areaSpan, areaExclusions;
    std::vector<std::string>   spawnConditions; // spawn_time, ai=[on_day_night_spawn], event_name…
    std::vector<NpcMakerEntry> npcComposition;
    SpawnBehavior              behavior;
    u16                        maxNpcCount{0};
};

/// Model the @code npcpos:territory@endcode entries.
struct SpawnArea
{
    struct Vertice { s32 x, y; };

    std::string           name;
    std::vector<Vertice>  vertices;
    std::vector<NpcMaker> subAreas;
    s32                   minZ, maxZ;
};

class SpawnManager
{
    friend void Orm::loadSpawnAreas();
    friend void Orm::loadNpcMakers();
    friend void Orm::loadFixedNpcMakers();
    friend void Orm::loadRandomNpcMakers();

private:
    SpawnManager() noexcept = default;

public:
    static auto instance() -> SpawnManager &;

public:
    auto spawnAreaCount() const -> size_t;

public:
    void load();
    void unload();

private:
    std::vector<SpawnArea> _spawnAreas;
    std::unordered_map<std::string, NpcMaker> _npcMakers;
};

#define sSpawnManager SpawnManager::instance()
