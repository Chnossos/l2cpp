/// @author    Chnossos
/// @date      Created on 2026-03-02

#include "Shortcut.hpp"

// Project includes
#include <l2cpp/network/Packet.hpp>

Network::Packet & operator<<(Network::Packet & p, Shortcut const & s)
{
    s.serialize(p);
    return p;
}

Shortcut::~Shortcut() = default;

Shortcut::Shortcut(u32 const index, ShortcutType const type)
    : _index{index}
    , _type{type}
{}

auto Shortcut::index() const -> Index        { return _index; }
auto Shortcut::type()  const -> ShortcutType { return _type;  }

void Shortcut::serialize(Network::Packet & p) const
{
    p << _type << _index;
    serializeImpl(p);
    p << 1; // ?
}
