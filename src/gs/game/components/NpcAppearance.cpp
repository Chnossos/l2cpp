/// @author    Chnossos
/// @date      Created on 2026-03-16

#include "NpcAppearance.hpp"

NpcAppearance::NpcAppearance() noexcept
    : _id(1) // Gremlin
    , _titleColor(0)
    , _nameIsVisible(true)
{}

auto NpcAppearance::id()            const -> u32 { return _id;            }
auto NpcAppearance::titleColor()    const -> u32 { return _titleColor;    }
bool NpcAppearance::nameIsVisible() const        { return _nameIsVisible; }

void NpcAppearance::setId           (u32  const id)      { _id            = id;      }
void NpcAppearance::setTitleColor   (u32  const color)   { _titleColor    = color;   }
void NpcAppearance::setNameIsVisible(bool const visible) { _nameIsVisible = visible; }
