/// @author    Chnossos
/// @date      Created on 2026-04-23

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/constants/ActorType.hpp>

// C++ includes
#include <filesystem>
#include <unordered_map>

namespace Orm
{
    void loadNpcs();
}

struct NpcInfo
{
    u32 id = 0;
    std::string name, title;
    u32 titleColor = 0xffffffff;
    ActorType type = ActorType::Npc;
    double baseSpeed = 100;

    double collisionHeight = 0, collisionRadius = 0;
    double xpRate = 1;
    StatValue maxHp = 1, maxMp = 1;
    StatValue hpRegen = 1, mpRegen = 1;
    u32 xp = 0, sp = 0;
    u8 level = 1;
    bool attackable = false;
};

class NpcDirectory
{
    friend void Orm::loadNpcs();

public:
    NpcDirectory() noexcept = default;

public:
    static void load();

public:
    static auto npcCount()     -> size_t;
    static auto monsterCount() -> size_t;
    static auto totalCount()   -> size_t;

    static auto find    (std::string_view   name) -> std::vector<Ref<NpcInfo const>>;
    static auto find    (std::wstring_view wName) -> std::vector<Ref<NpcInfo const>>;
    static auto find    (size_t id) -> OptRef<NpcInfo const>;
    static auto npc     (size_t id) -> OptRef<NpcInfo const>;
    static auto monster (size_t id) -> OptRef<NpcInfo const>;
    static auto maxLevel() -> u32;

private:
    static std::unordered_map<size_t, NpcInfo> _monsters;
    static std::unordered_map<size_t, NpcInfo> _npcs;
    static u32                                 _maxLevel;
};
