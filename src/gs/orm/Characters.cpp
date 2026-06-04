/// @author    Chnossos
/// @date      Created on 2026-05-09

#include "Characters.hpp"

// Project includes
#include <common/services/Database.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/CharacterStatus.hpp>
#include <gs/game/components/Position.hpp>
#include <gs/game/components/SkillDirectory.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/game/directories/CharacterTemplateDirectory.hpp>
#include <gs/game/directories/ItemTemplateDirectory.hpp>
#include <gs/game/inventory/Item.hpp>
#include <gs/game/inventory/ItemStorage.hpp>
#include <gs/game/ui/ActionShortcut.hpp>
#include <gs/game/ui/ItemShortcut.hpp>
#include <gs/game/ui/ShortcutBar.hpp>
#include <gs/game/ui/SkillShortcut.hpp>
#include <gs/utils/Conversion.hpp>

// Third-party includes
#include <gs/game/directories/ProfessionDirectory.hpp>
#include <spdlog/spdlog.h>

namespace
{
    void saveStatus(u32 characterId, Character const & c);

    void saveInventory(u32 characterId, Character const & c);
    void loadInventory(u32 characterId, Character & c);

    void saveShortcuts(u32 characterId, Character const & c);
    void loadShortcuts(u32 characterId, Character & c);
}

void Orm::loadProfessions()
try
{
    auto & professions = ProfessionDirectory::_professions;

    SQLite::Statement query(Database::instance(), R"(SELECT * FROM professions)");
    while (query.executeStep())
    {
        auto & p               = professions[            query.getColumn("id"               ).getUInt()];
        p.name                 =                         query.getColumn("name"             ).getString();
        p.profession           = static_cast<Profession>(query.getColumn("id"               ).getUInt());
        p.parentProfession     = static_cast<Profession>(query.getColumn("parent_id"        ).getUInt());
        p.canBeSubclassed      =                         query.getColumn("can_be_subclassed").getUInt();
        p.minimumLevel         = static_cast<u8        >(query.getColumn("minimum_level"    ).getUInt());
        p.maxHp                = static_cast<float     >(query.getColumn("max_hp"           ).getDouble());
        p.maxMp                = static_cast<float     >(query.getColumn("max_mp"           ).getDouble());
        p.maxCp                = static_cast<float     >(query.getColumn("max_cp"           ).getDouble());
        p.hpFlatPerLevel       = static_cast<float     >(query.getColumn("hp_flat_per_level").getDouble());
        p.mpFlatPerLevel       = static_cast<float     >(query.getColumn("mp_flat_per_level").getDouble());
        p.cpFlatPerLevel       = static_cast<float     >(query.getColumn("cp_flat_per_level").getDouble());
        p.hpMultiplierPerLevel = static_cast<float     >(query.getColumn("hp_mult_per_level").getDouble());
        p.mpMultiplierPerLevel = static_cast<float     >(query.getColumn("mp_mult_per_level").getDouble());
        p.cpMultiplierPerLevel = static_cast<float     >(query.getColumn("cp_mult_per_level").getDouble());
    }
}
catch (...)
{
    L2CPP_THROW_NESTED("Failed to load professions");
}

void Orm::loadCharacterTemplates()
{
    auto & templates = CharacterTemplateDirectory::_templates;

    SQLite::Statement query(Database::instance(), R"(SELECT * FROM character_templates)");
    while (query.executeStep())
    {
        auto & t             = templates[       query.getColumn("starting_profession"    ).getUInt()];
        t.collisionHeight[0] =                  query.getColumn("male_collision_height"  ).getDouble();
        t.collisionRadius[0] =                  query.getColumn("male_collision_radius"  ).getDouble();
        t.collisionHeight[1] =                  query.getColumn("female_collision_height").getDouble();
        t.collisionRadius[1] =                  query.getColumn("female_collision_radius").getDouble();
        t.STR                = static_cast<u8 >(query.getColumn("str"                    ).getUInt());
        t.DEX                = static_cast<u8 >(query.getColumn("dex"                    ).getUInt());
        t.CON                = static_cast<u8 >(query.getColumn("con"                    ).getUInt());
        t.INT                = static_cast<u8 >(query.getColumn("int"                    ).getUInt());
        t.WIT                = static_cast<u8 >(query.getColumn("wit"                    ).getUInt());
        t.MEN                = static_cast<u8 >(query.getColumn("men"                    ).getUInt());
        t.pAtk               = static_cast<u16>(query.getColumn("patk"                   ).getUInt());
        t.mAtk               = static_cast<u16>(query.getColumn("matk"                   ).getUInt());
        t.pDef               = static_cast<u16>(query.getColumn("pdef"                   ).getUInt());
        t.mDef               = static_cast<u16>(query.getColumn("mdef"                   ).getUInt());
        t.pAtkSpeed          = static_cast<u16>(query.getColumn("patk_speed"             ).getUInt());
        t.mAtkSpeed          = static_cast<u16>(query.getColumn("matk_speed"             ).getUInt());
        t.runSpeed           = static_cast<u16>(query.getColumn("run_speed"              ).getUInt());
        t.walkSpeed          = static_cast<u16>(query.getColumn("walk_speed"             ).getUInt());
    }
}

void Orm::saveCharacter(Character const & c)
{
    SQLite::Statement idQuery{Database::instance(), R"(
        SELECT id FROM characters WHERE name = :name LIMIT 1
    )"};
    idQuery.bind(":name", Utils::toString(c.name()));
    L2CPP_B_ASSERT(idQuery.executeStep(), "Failed to retrieve character id");
    auto const id = idQuery.getColumn("id").getUInt();

    SQLite::Statement query(Database::instance(), R"(
        UPDATE
            characters
        SET
            title       = :title
          , pos_x       = :pos_x
          , pos_y       = :pos_y
          , pos_z       = :pos_z
          , orientation = :orientation
        WHERE
            id = :character_id
    )");
    query.bind(":character_id", id);
    query.bind(":title",        Utils::toString(c.title()));
    query.bind(":pos_x",        c.position().x);
    query.bind(":pos_y",        c.position().y);
    query.bind(":pos_z",        c.position().z);
    query.bind(":orientation",  c.position().orientation);
    L2CPP_F_ASSERT([&] { query.exec(); }, "Failed to save character");

    saveStatus   (id, c);
    saveInventory(id, c);
    saveShortcuts(id, c);
}

void Orm::loadCharacter(Character & c)
{
    if (c.isFullyLoaded())
        return;

    // Here we do not load the preview fields again, we load the rest in order to enter the World
    SQLite::Statement query(Database::instance(), R"(
        SELECT
            id
          , title
          , orientation
        FROM
            characters
        WHERE
            name = :name
        LIMIT
            1
    )");
    query.bind(":name", Utils::toString(c.name()));
    L2CPP_B_ASSERT(query.executeStep(), "Failed to load character");

    c.setTitle(Utils::toWideString(query.getColumn("title").getString()));
    c.setOrientation(query.getColumn("orientation").getInt());

    auto const id = query.getColumn("id").getUInt();
    loadInventory(id, c);
    loadShortcuts(id, c);

    c.setIsFullyLoaded(true);
}

namespace
{
    void saveStatus(u32 const characterId, Character const & c)
    {
        auto query = SQLite::Statement(Database::instance(), R"(
            UPDATE
                character_statuses
            SET
                xp = :xp
              , sp = :sp
              , hp = :hp
              , mp = :mp
              , cp = :cp
            WHERE
                character_id   = :character_id
                AND profession = :profession
        )");
        query.bind(":character_id", characterId);
        query.bind(":xp",           c.status().xp());
        query.bind(":sp",           c.status().sp());
        query.bind(":hp",           static_cast<u32>(c.stats()[StatId::CurHp]));
        query.bind(":mp",           static_cast<u32>(c.stats()[StatId::CurMp]));
        query.bind(":cp",           static_cast<u32>(c.stats()[StatId::CurCp]));
        query.bind(":profession",   std::to_underlying(c.profession()));
        L2CPP_F_ASSERT([&] { query.exec(); }, "Failed to save character's profession");
    }

    void saveInventory(u32 const characterId, Character const & c)
    {
        SQLite::Transaction tr{Database::instance()};

        SQLite::Statement query{Database::instance(), R"(
            INSERT OR REPLACE INTO items
                ( id,  template_id,  owner_id,  storage_id,  quantity,  equipped,  enchant_level)
            VALUES
                (:id, :template_id, :owner_id,  0,          :quantity, :equipped, :enchant_level)
        )"};
        query.bind(":owner_id", characterId);

        for (Item const & item : c.inventory().items())
        {
            query.reset();
            query.bind(":id",          item.uid);
            query.bind(":template_id", item.tmplate.id);
            query.bind(":equipped",    item.equipped);

            if (item.tmplate.category == ItemCategory::Armor || item.tmplate.category == ItemCategory::Weapon)
            {
                query.bind(":quantity");
                query.bind(":enchant_level", item.enchantLevel);
            }
            else
            {
                query.bind(":quantity", item.quantity);
                query.bind(":enchant_level");
            }

            query.exec();
        }

        tr.commit();
    }

    void loadInventory(u32 const characterId, Character & c)
    {
        SQLite::Statement query{Database::instance(), R"(
            SELECT
                id
                 , template_id
                 , quantity
                 , enchant_level
            FROM
                items
            WHERE
                owner_id = :character_id AND storage_id = 0 AND (equipped IS NULL OR equipped == FALSE)
        )"};
        query.bind(":character_id", characterId);

        auto & inventory = c.inventory();
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
            auto const quantity     = query.getColumn("quantity");

            Item item{query.getColumn("id").getInt64()};
            item.tmplate      = std::move(*itemTemplate);
            item.enchantLevel = enchantLevel.isNull() ? 0 : static_cast<u8>(enchantLevel.getUInt());
            item.quantity     = quantity    .isNull() ? 1 : quantity.getUInt();

            inventory.add(std::move(item));
        }
    }

    void saveShortcuts(u32 const characterId, Character const & c)
    {
        auto const & bar = c.shortcutBar();

        static std::vector<Ref<Shortcut const>> nonEmptyShortcuts;
        nonEmptyShortcuts.reserve(bar.size());
        nonEmptyShortcuts.clear();

        std::vector<size_t> indexes;
        for (auto const & s : bar.shortcuts())
        {
            if (s)
            {
                nonEmptyShortcuts.emplace_back(s);
                indexes.emplace_back(s->index());
            }
        }

        // Delete all shortcuts that have been emptied during the game session (could be all of them)

        std::string queryText = R"(
            DELETE FROM
                character_shortcuts
            WHERE
                character_id   = :character_id
                AND profession = :profession
        )";

        if (!nonEmptyShortcuts.empty())
            queryText += std::format(R"( AND "index" NOT IN ({:n}))", indexes);

        SQLite::Transaction tr{Database::instance()};

        auto query = SQLite::Statement{Database::instance(), queryText };
        query.bind(":character_id", characterId);
        query.bind(":profession",   std::to_underlying(c.profession()));
        L2CPP_F_ASSERT([&] { query.exec(); }, "Failed to remove emptied shortcuts");

        // Update existing shortcut entries with current values
        query = SQLite::Statement{Database::instance(), R"(
            INSERT OR REPLACE INTO character_shortcuts
                ( character_id,  profession, "index",  type,  target_id)
            VALUES
                (:character_id, :profession, :index,  :type, :target_id)
        )"};

        for (Shortcut const & s : nonEmptyShortcuts)
        {
            query.reset();
            query.bind(":character_id", characterId);
            query.bind(":profession",   std::to_underlying(c.profession()));
            query.bind(":index",        s.index());
            query.bind(":type",         std::to_underlying(s.type()));
            query.bind(":target_id",    s.targetId());

            query.exec();
        }

        tr.commit();
    }

    void loadShortcuts(u32 const characterId, Character & c)
    {
        SQLite::Statement query{Database::instance(), std::format(R"(
            SELECT
                *
            FROM
                character_shortcuts
            WHERE
                character_id   = :character_id
                AND profession = :profession
        )")};
        query.bind(":character_id", characterId);
        query.bind(":profession",   std::to_underlying(c.profession()));

        while (query.executeStep())
        {
            auto const index    = query.getColumn("index"    ).getUInt();
            auto const type     = query.getColumn("type"     ).getUInt();
            auto const targetId = query.getColumn("target_id").getUInt();

            switch (static_cast<ShortcutType>(type))
            {
                case ShortcutType::Item:
                {
                    if (auto const item = c.inventory().find_if([=] (auto const & i) { return i.uid == targetId; }))
                        c.shortcutBar().set<ItemShortcut>(index, item);
                    else
                    {
                        SPDLOG_WARN("Failed to find item '{}' pointed at by shortcut in inventory, dropping shortcut",
                                   targetId);
                    }
                    break;
                }

                case ShortcutType::Skill:
                {
                    if (auto const skill = c.skills().skill(static_cast<SkillId>(targetId)))
                        c.shortcutBar().set<SkillShortcut>(index, skill->tmplate().uid());
                    else
                        SPDLOG_WARN("Failed to find skill '{}' pointed at by shortcut, dropping shortcut", targetId);

                    break;
                }

                case ShortcutType::Action:
                    c.shortcutBar().set<ActionShortcut>(index, targetId);
                    break;

                default:
                    SPDLOG_WARN("Unsupported shortcut type '{}', cannot restore shortcut", type);
                    break;
            }
        }
    }
}
