/// @author    Chnossos
/// @date      Created on 2026-03-11

#include "SkillTemplate.hpp"

// Project includes
#include "../actor/Actor.hpp"

#include <l2cpp/utils/Enum.hpp>

// C++ includes
#include <algorithm>
#include <ranges>

SkillTemplate::SkillTemplate(SkillId const id, std::string name, SkillLevel const lvl)
    : _id(id)
    , _level(lvl)
    , _type(SkillType::Unknown)
    , _targetType(SkillTargetType::Self)
    , _targetNature(SkillTargetNature::None)
    , _name(std::move(name))
    , _fullName(std::format("{} lv. {}", _name, _level))
    , _icon(std::format("icon.skill{:04}", _id))
    , _isMagic(false)
    , _castDuration(ClockDuration::zero())
    , _cooldownDuration(ClockDuration::zero())
{}

auto SkillTemplate::uid()          const -> SkillUid          { return {_id, _level}; }
auto SkillTemplate::id()           const -> SkillId           { return _id;           }
auto SkillTemplate::name()         const -> std::string_view  { return _name;         }
auto SkillTemplate::fullName()     const -> std::string_view  { return _fullName;     }
auto SkillTemplate::level()        const -> SkillLevel        { return _level;        }
auto SkillTemplate::type()         const -> SkillType         { return _type;         }
auto SkillTemplate::targetType()   const -> SkillTargetType   { return _targetType;   }
auto SkillTemplate::targetNature() const -> SkillTargetNature { return _targetNature; }
auto SkillTemplate::isMagic()      const -> bool              { return _isMagic;      }

auto SkillTemplate::needsTarget() const -> bool
{
    return l2cpp::Utils::Enum::isAnyOf(_targetType, SkillTargetType::Single, SkillTargetType::Multiple);
}

auto SkillTemplate::castDuration()     const -> ClockDuration     { return _castDuration;     }
auto SkillTemplate::cooldownDuration() const -> ClockDuration     { return _cooldownDuration; }

auto SkillTemplate::effects() const -> std::span<std::unique_ptr<AbnormalEffectFactory> const> { return _effects; }

void SkillTemplate::setType(SkillType const type)                     { _type = type;                 }
void SkillTemplate::setTargetType(SkillTargetType const type)         { _targetType = type;           }
void SkillTemplate::setTargetNature(SkillTargetNature const nature)   { _targetNature = nature;       }
void SkillTemplate::setIsMagic(bool const isMagic)                    { _isMagic = isMagic;           }
void SkillTemplate::setCastDuration(ClockDuration const castDuration) { _castDuration = castDuration; }

void SkillTemplate::applyEffects(Actor & source, Actor & target) const
{
    auto & effects = target.abnormalEffects();

    // FIXME: do not apply buff if already applied but applied power is superior than the one we try to apply (got it?)

    auto v = effects | std::views::filter([this] (auto const & effect) { return effect->skillUid() == uid(); });
    std::ranges::for_each(v, [] (auto const & effect) { return effect->cancel(); });

    if (!v.empty() && _type == SkillType::Toggle)
        return; // Avoid restarting a skill we just toggled off

    for (auto const & e : _effects)
        e->apply(source, target);
}
