/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include <common/utils/Enum.hpp>
#include <gs/game/World.hpp>
#include <gs/game/lobby/CharacterCreationParameters.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/lobby/CharacterCreatePacket.hpp>

// C++ includes
#include <regex>

DECLARE_PACKET_HANDLER(CharacterList)

DEFINE_PACKET_HANDLER(CharacterCreate) try
{
    CharacterCreationParameters params;

    PacketReader reader(player.connection().readBuffer().subspan(3));
    reader
        >> params.name
        >> params.race
        >> params.sex
        >> params.profession
        >> params.INT
        >> params.STR
        >> params.CON
        >> params.MEN
        >> params.DEX
        >> params.WIT
        >> params.hairStyle
        >> params.hairColor
        >> params.face
    ;

    using enum Race;
    using Utils::Enum::isAnyOf;
    L2CPP_B_ASSERT(isAnyOf(params.race, Human, Elf, DarkElf, Orc, Dwarf),
                   "Invalid race value '{}'", std::to_underlying(params.race));

    using enum Sex;
    L2CPP_B_ASSERT(isAnyOf(params.sex, Male, Female), "Invalid sex value '{}'", std::to_underlying(params.sex));

    bool isValidProfessionForRace = false;
    switch (params.race)
    {
#define CHECK_PROFESSION(race, ...) case race: isValidProfessionForRace = isAnyOf(params.profession, __VA_ARGS__); break

        using enum Profession;
        CHECK_PROFESSION(Human,   HumanFighter, HumanMystic);
        CHECK_PROFESSION(Elf,     ElvenFighter, ElvenMystic);
        CHECK_PROFESSION(DarkElf, DarkFighter,  DarkMystic);
        CHECK_PROFESSION(Orc,     OrcFighter,   OrcMystic);
        CHECK_PROFESSION(Dwarf,   DwarvenFighter);

#undef  CHECK_PROFESSION

        default:
            break;
    }

    L2CPP_B_ASSERT(isValidProfessionForRace, "Invalid profession value '{}' for race '{}'",
                   std::to_underlying(params.profession), std::to_underlying(params.race));

    // TODO: check hair style/color + face values for each race & sex combinations

    std::wregex const namePattern{LR"([A-Za-z0-9]{1,16})"};

    CharacterCreationResult result;
    if (std::regex_match(params.name, namePattern))
        result = World::createCharacter(player, params);
    else
    {
        result = CharacterCreationResult::InvalidName;
        SPDLOG_TRACE(L"Player attempted to name their new character '{}' which breaks naming rules", params.name);
    }

    player.connection().send(CharacterCreatePacket{result});
    if (result == CharacterCreationResult::Success)
        handleCharacterList(player);
}
catch (...)
{
    player.connection().send(CharacterCreatePacket{CharacterCreationResult::Failure});
    L2CPP_THROW_NESTED("Failed to create character");
}
