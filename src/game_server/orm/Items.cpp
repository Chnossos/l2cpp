/// @author    Chnossos
/// @date      Created on 2026-05-13

#include "Items.hpp"

// Project includes
#include <l2cpp/services/Database.hpp>

void Orm::loadItemTemplates(std::unordered_map<decltype(ItemTemplate::id), ItemTemplate> & templates)
{
    SQLite::Statement query(Database::instance(), R"(
        SELECT
            (SELECT COUNT(*) FROM item_templates) AS size
          , *
        FROM
            item_templates
    )");

    while (query.executeStep())
    {
        templates.reserve(query.getColumn("size").getUInt());
        templates.try_emplace(query.getColumn("id").getUInt()
          , query.getColumn("id"  ).getUInt()
          , query.getColumn("name").getString()
          , static_cast<ItemCategory>(query.getColumn("category" ).getUInt())
          , static_cast<ItemGrade   >(query.getColumn("grade"    ).getUInt())
          , static_cast<GearSlot    >(query.getColumn("gear_slot").getUInt())
        );
    }
}
