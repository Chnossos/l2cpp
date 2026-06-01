/// @author    Chnossos
/// @date      Created on 2026-03-16

#pragma once

// Project includes
#include "Actor.hpp"

class NpcAppearance;
class NpcStatus;

class Npc : public Actor
{
public:
    explicit Npc(u32 id);

protected:
    Npc(ActorType type, u32 id);

public:
    auto appearance()       -> NpcAppearance       &;
    auto appearance() const -> NpcAppearance const &;
    auto status()     const -> NpcStatus     const &;

    bool isAttackable() const override { return false; }
};
