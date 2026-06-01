/// @author    Chnossos
/// @date      Created on 2026-03-16

#pragma once

// Project includes
#include <gs/game/actor/Npc.hpp>

class Monster : public Npc
{
public:
    explicit Monster(u32 id) noexcept;

public:
    bool isAttackable() const override;
};
