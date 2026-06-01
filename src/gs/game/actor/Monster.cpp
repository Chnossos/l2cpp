/// @author    Chnossos
/// @date      Created on 2026-03-16

#include "Monster.hpp"

Monster::Monster(u32 const id) noexcept
    : Npc(ActorType::Monster, id)
{}

bool Monster::isAttackable() const { return true; }
