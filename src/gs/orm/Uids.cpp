/// @author    Chnossos
/// @date      Created on 2026-05-14

#include "Uids.hpp"

// Project includes
#include <common/services/Database.hpp>
#include <gs/game/inventory/Item.hpp>

void Orm::loadUids()
{
    Item::nextId = Database::instance().execAndGet("SELECT MAX(id)+1 FROM items").getInt64();
}
