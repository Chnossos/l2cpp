/// @author    Chnossos
/// @date      Created on 2026-05-18

#pragma once

// Project includes
#include <gs/game/skill/SkillUid.hpp>
#include <gs/game/ui/Shortcut.hpp>

class SkillShortcut : public Shortcut
{
public:
    explicit SkillShortcut(Index index, SkillUid skillUid);

public:
    auto targetId()  const -> u32 override;

private:
    void serializeImpl(Network::Packet &) const override;

private:
    SkillUid _skillUid;
};
