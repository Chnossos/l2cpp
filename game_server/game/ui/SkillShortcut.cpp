/// @author    Chnossos
/// @date      Created on 2026-05-18

#include "SkillShortcut.hpp"

// Project includes
#include <l2cpp/network/Packet.hpp>

SkillShortcut::SkillShortcut(Index const index, SkillUid const skillUid)
    : Shortcut{index, ShortcutType::Skill}
    , _skillUid{skillUid}
{}

auto SkillShortcut::targetId() const -> u32
{
    return _skillUid.id();
}

void SkillShortcut::serializeImpl(Network::Packet & p) const
{
    p
        << static_cast<u32>(_skillUid.id())
        << static_cast<u32>(_skillUid.level())
    ;
}
