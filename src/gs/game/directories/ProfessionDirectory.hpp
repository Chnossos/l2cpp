/// @author    Chnossos
/// @date      Created on 2026-06-03

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/components/Position.hpp>
#include <gs/game/constants/Profession.hpp>

class Actor;

namespace Orm
{
    void loadProfessions();
}

class ProfessionDirectory
{
    friend void Orm::loadProfessions();

    struct ProfessionInfo
    {
        std::vector<Position> startingLocations;

        std::string name;
        float       maxHp{}, hpFlatPerLevel{}, hpMultiplierPerLevel{};
        float       maxMp{}, mpFlatPerLevel{}, mpMultiplierPerLevel{};
        float       maxCp{}, cpFlatPerLevel{}, cpMultiplierPerLevel{};

        std::optional<Profession> parentProfession;
        Profession                profession{};
        u8                        minimumLevel{1};
        bool                      canBeSubclassed{};

        void applyBaseStats(Actor &) const;
    };

public:
    ProfessionDirectory() = delete;

public:
    static auto count() -> size_t;
    static auto find              (Profession) -> OptRef<ProfessionInfo const>;
    static auto parent            (Profession) -> OptRef<ProfessionInfo const>;
    static auto startingProfession(Profession) -> OptRef<ProfessionInfo const>;
    static auto rank              (Profession) -> u8;

public:
    static void load();

private:
    static std::unordered_map<Profession, ProfessionInfo> _professions;
};
