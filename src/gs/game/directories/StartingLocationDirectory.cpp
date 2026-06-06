/// @author    Chnossos
/// @date      Created on 2026-06-05

#include "StartingLocationDirectory.hpp"

// Project includes
#include <core/Exception.hpp>

// C++ includes
#include <numeric>

auto StartingLocationDirectory::count() -> size_t
{
    auto const size = _globalLocations.positions.size();
    return std::accumulate(_professionLocations.cbegin(), _professionLocations.cend(), size,
                           [] (auto const val, auto const & p) { return val + p.second.positions.size(); });
}

auto StartingLocationDirectory::get(Profession const profession) -> Position const &
{
    if (!_globalLocations.positions.empty())
    {
        auto const idx = _globalLocations.cursor++;
        _globalLocations.cursor %= _globalLocations.positions.size();
        return _globalLocations.positions[idx];
    }
    else
    {
        auto const it = _professionLocations.find(profession);
        L2CPP_B_ASSERT(it != _professionLocations.end(),
                       "Can't find any starting location for profession '{}'", std::to_underlying(profession));

        auto const idx = it->second.cursor++;
        it->second.cursor %= it->second.positions.size();
        return it->second.positions[idx];
    }
}

void StartingLocationDirectory::load()
{
    Orm::loadStartingLocations();
}

StartingLocationDirectory::Locations                                 StartingLocationDirectory::_globalLocations;
std::unordered_map<Profession, StartingLocationDirectory::Locations> StartingLocationDirectory::_professionLocations;
