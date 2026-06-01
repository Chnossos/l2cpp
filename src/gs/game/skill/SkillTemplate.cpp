/// @author    Chnossos
/// @date      Created on 2026-03-11

#include "SkillTemplate.hpp"

// Project includes
#include "../actor/Actor.hpp"

#include <common/utils/Enum.hpp>

// C++ includes
#include <algorithm>
#include <ranges>

SkillTemplate::SkillTemplate(SkillId const id, std::string name, SkillLevel const lvl)
    : _id(id)
    , _level(lvl)
    , _operatingType(SkillOperatingType::Unknown)
    , _name(std::move(name))
    , _fullName(std::format("{} lv. {}", _name, _level))
    , _icon(std::format("icon.skill{:04}", _id))
    , _isMagic(false)
    , _castDuration(ClockDuration::zero())
    , _cooldownDuration(ClockDuration::zero())
{}

auto SkillTemplate::uid()           const -> SkillUid           { return {_id, _level};  }
auto SkillTemplate::id()            const -> SkillId            { return _id;            }
auto SkillTemplate::name()          const -> std::string_view   { return _name;          }
auto SkillTemplate::fullName()      const -> std::string_view   { return _fullName;      }
auto SkillTemplate::level()         const -> SkillLevel         { return _level;         }
auto SkillTemplate::operatingType() const -> SkillOperatingType { return _operatingType; }
auto SkillTemplate::targetType()    const -> EffectTargetType
{
    return _effects.empty() ? EffectTargetType::Self : _effects.front()->targetType();
}

auto SkillTemplate::targetNature() const -> SkillTargetNature
{
    return _effects.empty() ? SkillTargetNature::None : _effects.front()->targetNature();
}

auto SkillTemplate::needsTarget() const -> bool
{
    return Utils::Enum::isAnyOf(targetType(), EffectTargetType::Single, EffectTargetType::Multiple);
}

auto SkillTemplate::isMagic()          const -> bool          { return _isMagic;          }
auto SkillTemplate::castDuration()     const -> ClockDuration { return _castDuration;     }
auto SkillTemplate::cooldownDuration() const -> ClockDuration { return _cooldownDuration; }

auto SkillTemplate::effects() const -> std::span<std::unique_ptr<EffectFactory> const> { return _effects; }

void SkillTemplate::setOperatingType(SkillOperatingType const type)   { _operatingType = type;         }
void SkillTemplate::setIsMagic(bool const isMagic)                    { _isMagic       = isMagic;      }
void SkillTemplate::setCastDuration(ClockDuration const castDuration) { _castDuration  = castDuration; }

void SkillTemplate::addEffectFactoryImpl(std::unique_ptr<EffectFactory> factory)
{
    _effects.emplace_back(std::move(factory));
}
