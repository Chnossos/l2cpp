/// @author    Chnossos
/// @date      Created on 2026-06-05

#pragma once

// Project includes
#include <gs/game/components/Position.hpp>
#include <gs/game/constants/Profession.hpp>

// C++ includes
#include <vector>

namespace Orm
{
    void loadStartingLocations();
}

class StartingLocationDirectory
{
    friend void Orm::loadStartingLocations();

public:
    struct Locations
    {
        std::vector<Position> positions;
        size_t                cursor{};
    };

public:
    static auto count()         -> size_t;
    static auto get(Profession) -> Position const &;

public:
    static void load();

private:
    static Locations                                 _globalLocations;
    static std::unordered_map<Profession, Locations> _professionLocations;
};
