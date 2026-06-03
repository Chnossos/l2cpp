/// @author    Chnossos
/// @date      Created on 2026-06-03

#include "ProfessionDirectory.hpp"

// Project includes
// #include <gs/orm/Characters.hpp>

auto ProfessionDirectory::count() -> size_t
{
    return _professions.size();
}

auto ProfessionDirectory::find(Profession const profession) -> OptRef<ProfessionInfo const>
{
    auto const it = _professions.find(std::to_underlying(profession));
    return it != _professions.end() ? OptRef<ProfessionInfo const>{it->second} : std::nullopt;
}

void ProfessionDirectory::load()
{
    Orm::loadProfessions();
}

std::unordered_map<u32, ProfessionDirectory::ProfessionInfo> ProfessionDirectory::_professions;
