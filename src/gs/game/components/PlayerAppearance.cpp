/// @author    Chnossos
/// @date      Created on 2026-05-08

#include "PlayerAppearance.hpp"

// Project includes
#include <common/core/Exception.hpp>
#include <common/utils/Enum.hpp>

PlayerAppearance::PlayerAppearance() noexcept
  : _race(Race::Human)
  , _startingProfession(Profession::HumanFighter)
  , _sex(Sex::Male)
  , _hairStyle(0)
  , _hairColor(0)
  , _face(0)
  , _nameColor(0xffffffff)
{}

auto PlayerAppearance::race()               const -> Race       { return _race;               }
auto PlayerAppearance::startingProfession() const -> Profession { return _startingProfession; }
auto PlayerAppearance::sex()                const -> Sex        { return _sex;                }
auto PlayerAppearance::hairStyle()          const -> u32        { return _hairStyle;          }
auto PlayerAppearance::hairColor()          const -> u32        { return _hairColor;          }
auto PlayerAppearance::face()               const -> u32        { return _face;               }
auto PlayerAppearance::nameColor()          const -> u32        { return _nameColor;          }

void PlayerAppearance::setStartingProfession(Profession const p)
{
    using enum Profession;
    L2CPP_B_ASSERT(Utils::Enum::isAnyOf(p, HumanFighter, ElvenFighter, DarkFighter, OrcFighter, DwarvenFighter,
                                           HumanMystic,  ElvenMystic,  DarkMystic,  OrcMystic),
                   "Profession '{}' is not a valid starting profession", std::to_underlying(p));

    _startingProfession = p;

    static std::unordered_map<Profession, Race> const professionToRace
    {
        {HumanFighter,   Race::Human  },
        {HumanMystic,    Race::Human  },
        {ElvenFighter,   Race::Elf    },
        {ElvenMystic,    Race::Elf    },
        {DarkFighter,    Race::DarkElf},
        {DarkMystic,     Race::DarkElf},
        {OrcFighter,     Race::Orc    },
        {OrcMystic,      Race::Orc    },
        {DwarvenFighter, Race::Dwarf  },
    };
    _race = professionToRace.at(p);
}

void PlayerAppearance::setSex      (Sex const sex)       { _sex       = sex;       }
void PlayerAppearance::setHairStyle(u32 const hairStyle) { _hairStyle = hairStyle; }
void PlayerAppearance::setHairColor(u32 const hairColor) { _hairColor = hairColor; }
void PlayerAppearance::setFace     (u32 const face)      { _face      = face;      }
void PlayerAppearance::setNameColor(u32 const nameColor) { _nameColor = nameColor; }
