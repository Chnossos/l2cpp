/// @author    Chnossos
/// @date      Created on 2026-05-13

#include "ItemTemplateDirectory.hpp"

// Project includes
#include <gs/orm/Items.hpp>

auto ItemTemplateDirectory::size() -> size_t
{
    return _templates.size();
}

auto ItemTemplateDirectory::find(TemplateId const id) -> OptRef<ItemTemplate const>
{
    auto const it = _templates.find(id);
    return it != _templates.end() ? OptRef{it->second} : std::nullopt;
}

void ItemTemplateDirectory::load()
{
    Orm::loadItemTemplates(_templates);
}

std::unordered_map<decltype(ItemTemplate::id), ItemTemplate> ItemTemplateDirectory::_templates;
