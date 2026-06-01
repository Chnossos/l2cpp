/// @author    Chnossos
/// @date      Created on 2026-05-13

#pragma once

// Project includes
#include "ItemTemplate.hpp"

// C++ includes
#include <unordered_map>

class ItemTemplateDirectory
{
    using TemplateId = decltype(ItemTemplate::id);

    ItemTemplateDirectory() noexcept = default;

public:
    static auto size() -> size_t;
    static auto find(TemplateId) -> OptRef<ItemTemplate>;

public:
    static void load();

private:
    static std::unordered_map<TemplateId, ItemTemplate> _templates;
};
