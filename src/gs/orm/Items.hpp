/// @author    Chnossos
/// @date      Created on 2026-05-13

#pragma once

// Project includes
#include "../game/inventory/ItemTemplate.hpp"

// C++ includes
#include <unordered_map>

class ItemTemplateDirectory;

namespace Orm
{
    void loadItemTemplates(std::unordered_map<decltype(ItemTemplate::id), ItemTemplate> &);
}
