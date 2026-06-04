/// @author    Chnossos
/// @date      Created on 2026-06-02

#include "CharacterTemplateDirectory.hpp"

// Project includes
#include <core/Exception.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/PlayerAppearance.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/game/constants/StatId.hpp>

void CharacterTemplateDirectory::CharacterTemplate::apply(Character & c) const
{
    auto & a = c.appearance();
    a.setCollisionHeight(collisionHeight[std::to_underlying(a.sex())]);
    a.setCollisionRadius(collisionRadius[std::to_underlying(a.sex())]);

    using enum StatId;
    auto & stats = c.stats();
    stats[BaseStr      ] = STR;
    stats[BaseDex      ] = DEX;
    stats[BaseCon      ] = CON;
    stats[BaseInt      ] = INT;
    stats[BaseWit      ] = WIT;
    stats[BaseMen      ] = MEN;
    stats[BasePAtk     ] = pAtk;
    stats[BaseMAtk     ] = mAtk;
    stats[BasePDef     ] = pDef;
    stats[BaseMDef     ] = mDef;
    stats[BasePAtkSpeed] = pAtkSpeed;
    stats[BaseMAtkSpeed] = mAtkSpeed;
    stats[BaseRunSpeed ] = runSpeed;
    stats[BaseWalkSpeed] = walkSpeed;
    stats.compute(c);
}

auto CharacterTemplateDirectory::count() -> size_t
{
    return _templates.size();
}

auto CharacterTemplateDirectory::find(Profession const profession) -> OptRef<CharacterTemplate const>
{
    auto const it = _templates.find(std::to_underlying(profession));
    return it != _templates.end() ? OptRef<CharacterTemplate const>{it->second} : std::nullopt;
}

void CharacterTemplateDirectory::load()
{
    Orm::loadCharacterTemplates();
}

std::unordered_map<u32, CharacterTemplateDirectory::CharacterTemplate> CharacterTemplateDirectory::_templates;
