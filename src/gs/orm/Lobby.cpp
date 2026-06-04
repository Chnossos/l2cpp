/// @author    Chnossos
/// @date      Created on 2026-05-06

#include "Lobby.hpp"

// Project includes
#include <common/services/Database.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/CharacterSelectionData.hpp>
#include <gs/game/components/CharacterStatus.hpp>
#include <gs/game/components/Gear.hpp>
#include <gs/game/components/PlayerAppearance.hpp>
#include <gs/game/components/Position.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/game/directories/CharacterTemplateDirectory.hpp>
#include <gs/game/directories/ItemTemplateDirectory.hpp>
#include <gs/game/inventory/ItemStorage.hpp>
#include <gs/utils/Conversion.hpp>

// Third-party includes
#include <spdlog/spdlog.h>

static void loadGear(u32 characterId, Character & c);

auto Orm::loadCharacterPreviews(AccountId const accountId) -> std::vector<std::unique_ptr<Character>>
try
{
    SQLite::Statement query(Database::instance(), R"(
        SELECT
            id
          , name
          , starting_profession
          , sex
          , hair_style
          , hair_color
          , face
          , pos_x
          , pos_y
          , pos_z
          , current_profession
          , xp
          , sp
          , hp
          , mp
          , cp
          , selected
        FROM
            characters
        JOIN
            character_statuses AS status
            ON  status.character_id = characters.id
            AND status.profession   = characters.current_profession
        JOIN
            character_previews previews ON previews.character_id = characters.id
        WHERE
            account_id = :account_id
        ORDER BY
            creation_date
    )");
    query.bind(":account_id", accountId);

    std::vector<std::unique_ptr<Character>> previews;
    while (query.executeStep())
    {
        auto & c = *previews.emplace_back(std::make_unique<Character>());
        c.setName(Utils::toWideString(query.getColumn("name").getString()));

        auto & a = c.appearance();
        a.setStartingProfession(static_cast<Profession>(query.getColumn("starting_profession").getUInt()));
        a.setSex               (static_cast<Sex>(query.getColumn("sex").getUInt()));
        a.setHairStyle         (query.getColumn("hair_style").getUInt());
        a.setHairColor         (query.getColumn("hair_color").getUInt());
        a.setFace              (query.getColumn("face"      ).getUInt());

        if (auto const charTemplate = CharacterTemplateDirectory::find(a.startingProfession()))
            charTemplate->apply(c);

        c.setPosX(query.getColumn("pos_x").getInt());
        c.setPosY(query.getColumn("pos_y").getInt());
        c.setPosZ(query.getColumn("pos_z").getInt());

        c.setProfession(static_cast<Profession>(query.getColumn("current_profession").getUInt()));
        c.status().setXp(query.getColumn("xp").getUInt());
        c.status().setSp(query.getColumn("sp").getUInt());

        auto & data   = c.addComponent<CharacterSelectionData>();
        data.selected = query.getColumn("selected").getUInt();

        auto & stats = c.stats();
        stats[StatId::CurHp] = query.getColumn("hp").getUInt();
        stats[StatId::CurMp] = query.getColumn("mp").getUInt();
        stats[StatId::CurCp] = query.getColumn("cp").getUInt();

        loadGear(query.getColumn("id").getUInt(), c);
    }
    return previews;
}
catch (SQLite::Exception const & e)
{
    SPDLOG_ERROR("Failed to fetch characters for account ID '{}': SQL error '{}' (code: {})",
                  accountId, e.what(), e.getErrorCode());
    return {};
}

void Orm::createCharacter(AccountId const accountId, Character & c) try
{
    SQLite::Transaction tr(Database::instance());

    SQLite::Statement query(Database::instance(), R"(
        INSERT INTO characters (
            account_id
          , name
          , starting_profession
          , sex
          , hair_style
          , hair_color
          , face
          , pos_x
          , pos_y
          , pos_z
          , current_profession
        ) VALUES (
            :account_id
          , :name
          , :starting_profession
          , :sex
          , :hair_style
          , :hair_color
          , :face
          , :pos_x
          , :pos_y
          , :pos_z
          , :current_profession
        )
    )");
    query.bind(":account_id",          accountId);
    query.bind(":name",                Utils::toString(c.name()));
    query.bind(":starting_profession", std::to_underlying(c.appearance().startingProfession()));
    query.bind(":sex",                 std::to_underlying(c.appearance().sex()));
    query.bind(":hair_style",          c.appearance().hairStyle());
    query.bind(":hair_color",          c.appearance().hairColor());
    query.bind(":face",                c.appearance().face());
    query.bind(":pos_x",               c.position().x);
    query.bind(":pos_y",               c.position().y);
    query.bind(":pos_z",               c.position().z);
    query.bind(":current_profession",  std::to_underlying(c.profession()));
    L2CPP_F_ASSERT([&] { query.exec(); }, "Failed to insert a new character");

    auto const charId = static_cast<u32>(Database::instance().getLastInsertRowid());

    query = SQLite::Statement(Database::instance(), R"(
        INSERT INTO character_professions VALUES (:character_id, :profession)
    )");
    query.bind(":character_id", charId);
    query.bind(":profession",   std::to_underlying(c.profession()));
    L2CPP_F_ASSERT([&] { query.exec(); }, "Failed to insert character profession");

    query = SQLite::Statement(Database::instance(), R"(
        INSERT
            INTO character_statuses ( character_id,  profession,  hp,  mp,  cp)
            VALUES                  (:character_id, :profession, :hp, :mp, :cp)
    )");
    query.bind(":character_id", charId);
    query.bind(":profession",   std::to_underlying(c.profession()));
    query.bind(":hp",           static_cast<u32>(c.stats()[StatId::MaxHp]));
    query.bind(":mp",           static_cast<u32>(c.stats()[StatId::MaxMp]));
    query.bind(":cp",           static_cast<u32>(c.stats()[StatId::MaxCp]));
    L2CPP_F_ASSERT([&] { query.exec(); }, "Failed to insert character status");

    tr.commit();

    loadGear(charId, c);
}
catch (SQLite::Exception const & e)
{
    L2CPP_THROW(e.getErrorCode(), "SQL error '{}'", e.what());
}

void Orm::selectCharacter(AccountId const accountId, std::wstring_view const selectedCharName)
try
{
    SQLite::Transaction tr(Database::instance());

    SQLite::Statement query(Database::instance(), R"(
        UPDATE
            character_previews
        SET
            selected = FALSE
        WHERE
            character_id IN (SELECT id FROM characters WHERE account_id = :account_id)
    )");
    query.bind(":account_id", accountId);
    L2CPP_F_ASSERT([&] { query.exec(); }, "Failed to unselect all characters");

    query = SQLite::Statement(Database::instance(), R"(
        UPDATE
            character_previews
        SET
            selected = TRUE
        WHERE
            character_id = (SELECT id FROM characters WHERE name = :name LIMIT 1)
    )");
    query.bind(":name", Utils::toString(selectedCharName));
    L2CPP_F_ASSERT([&] { query.exec(); }, "Failed to select character");

    tr.commit();
}
catch (SQLite::Exception const & e)
{
    L2CPP_THROW(e.getErrorCode(), "SQL error '{}'", e.what());
}

void Orm::deleteCharacter(std::wstring_view const charName)
try
{
    SQLite::Statement query(Database::instance(), R"(
        DELETE FROM
            characters
        WHERE
            id = (SELECT id FROM characters WHERE name = :name LIMIT 1)
    )");
    query.bind(":name", Utils::toString(charName));
    L2CPP_F_ASSERT([&] { query.exec(); }, "Failed to delete character");
}
catch (SQLite::Exception const & e)
{
    L2CPP_THROW(e.getErrorCode(), "SQL error '{}'", e.what());
}

void loadGear(u32 const characterId, Character & c)
{
    SQLite::Statement query{Database::instance(), R"(
        SELECT
            id
          , template_id
          , enchant_level
        FROM
            items
        WHERE
            owner_id = :character_id AND equipped = TRUE
    )"};
    query.bind(":character_id", characterId);

    auto & inventory = c.inventory();
    auto & gear      = c.gear();
    while (query.executeStep())
    {
        auto const templateId   = query.getColumn("template_id").getUInt();
        auto const itemTemplate = ItemTemplateDirectory::find(templateId);
        if (!itemTemplate)
        {
            SPDLOG_ERROR("ItemTemplate id '{}' not loaded but a character requests it", templateId);
            continue;
        }

        auto const enchantLevel = query.getColumn("enchant_level");

        Item item{query.getColumn("id").getInt64()};
        item.tmplate      = std::move(*itemTemplate);
        item.enchantLevel = static_cast<u8>(enchantLevel.isNull() ? 0 : enchantLevel.getUInt());

        gear.equipItem(inventory.add(std::move(item)));
    }
}
